#include "se.h"

#include "mapldr.h"
#include "sebasesup.h"
#include "senative.h"
#include "process.h"


#include <string>
#include <string_view>
#include <charconv>
#include <limits>
#include <mutex>
#include <unordered_set>

static BOOLEAN SepValidateExportTableRvas(
    _In_ PIMAGE_NT_HEADERS ImageNtHeader,
    _In_ PIMAGE_EXPORT_DIRECTORY ExportDirectory
)
{
    ULONG imageSize;
    ULONG addressTableSize;
    ULONG nameTableSize;
    ULONG ordinalTableSize;
    ULONG addressTableEnd;
    ULONG nameTableEnd;
    ULONG ordinalTableEnd;

    imageSize = ImageNtHeader->OptionalHeader.SizeOfImage;

    if (ExportDirectory->NumberOfFunctions > 0)
    {
        if (ExportDirectory->AddressOfFunctions == 0)
            return FALSE;
        if (!NT_SUCCESS(RtlULongMult(ExportDirectory->NumberOfFunctions, sizeof(ULONG), &addressTableSize)))
            return FALSE;
        if (!NT_SUCCESS(RtlULongAdd(ExportDirectory->AddressOfFunctions, addressTableSize, &addressTableEnd)))
            return FALSE;
        if (addressTableEnd > imageSize)
            return FALSE;
    }

    if (ExportDirectory->NumberOfNames > 0)
    {
        if (ExportDirectory->AddressOfNames == 0)
            return FALSE;
        if (!NT_SUCCESS(RtlULongMult(ExportDirectory->NumberOfNames, sizeof(ULONG), &nameTableSize)))
            return FALSE;
        if (!NT_SUCCESS(RtlULongAdd(ExportDirectory->AddressOfNames, nameTableSize, &nameTableEnd)))
            return FALSE;
        if (nameTableEnd > imageSize)
            return FALSE;
    }

    if (ExportDirectory->NumberOfNames > 0)
    {
        if (ExportDirectory->AddressOfNameOrdinals == 0)
            return FALSE;
        if (!NT_SUCCESS(RtlULongMult(ExportDirectory->NumberOfNames, sizeof(USHORT), &ordinalTableSize)))
            return FALSE;
        if (!NT_SUCCESS(RtlULongAdd(ExportDirectory->AddressOfNameOrdinals, ordinalTableSize, &ordinalTableEnd)))
            return FALSE;
        if (ordinalTableEnd > imageSize)
            return FALSE;
    }

    return TRUE;
}


static NTSTATUS SepGetExportDirectory(
    _In_ PVOID BaseAddress,
    _Out_ PIMAGE_NT_HEADERS* ImageNtHeader,
    _Out_ PIMAGE_DATA_DIRECTORY* DataDirectory,
    _Out_ PIMAGE_EXPORT_DIRECTORY* ExportDirectory
)
{
    NTSTATUS status;

    status = SeGetLoaderEntryImageNtHeaders(BaseAddress, ImageNtHeader);

    if (!NT_SUCCESS(status))
        return status;

    return SeGetLoaderEntryImageDirectory(
        BaseAddress,
        *ImageNtHeader,
        IMAGE_DIRECTORY_ENTRY_EXPORT,
        DataDirectory,
        (PVOID*)ExportDirectory,
        NULL
    );
}

static ULONG SepLookupLoaderEntryImageExportFunctionIndex(
    _In_ PVOID BaseAddress,
    _In_ PIMAGE_NT_HEADERS ImageNtHeader,
    _In_ PIMAGE_EXPORT_DIRECTORY ExportDirectory,
    _In_ PULONG ExportNameTable,
    _In_ PCSTR ExportName
)
{
    LONG low;
    LONG high;
    LONG i;
    ULONG imageSize;

    if (ExportDirectory->NumberOfNames == 0)
        return ULONG_MAX;

    imageSize = ImageNtHeader->OptionalHeader.SizeOfImage;
    low = 0;
    high = ExportDirectory->NumberOfNames - 1;

    do
    {
        PSTR name;
        INT comparison;
        ULONG nameRva;

        i = (low + high) / 2;
        nameRva = ExportNameTable[i];

        if (nameRva == 0 || nameRva >= imageSize)
            return ULONG_MAX;

        name = static_cast<PSTR>(PTR_ADD_OFFSET(BaseAddress, nameRva));
        comparison = strcmp(ExportName, name);

        if (comparison == 0)
            return i;
        else if (comparison < 0)
            high = i - 1;
        else
            low = i + 1;
    } while (low <= high);

    return ULONG_MAX;
}

static PVOID SepResolveExportForwarder(
    _In_ PVOID BaseAddress,
    _In_ PVOID ExportAddress,
    _In_ PIMAGE_DATA_DIRECTORY DataDirectory,
    _In_ PIMAGE_EXPORT_DIRECTORY ExportDirectory
)
{
    UNREFERENCED_PARAMETER(BaseAddress);

    const auto exportStart = reinterpret_cast<const std::byte*>(ExportDirectory);
    const auto exportEnd = exportStart + DataDirectory->Size;
    const auto exportPtr = reinterpret_cast<const std::byte*>(ExportAddress);

    if (exportPtr < exportStart || exportPtr >= exportEnd)
        return ExportAddress;

    std::string_view forwarder{ reinterpret_cast<const char*>(ExportAddress) };

    const std::size_t dotPos = forwarder.find('.');
    if (dotPos == std::string_view::npos)
        return ExportAddress;

    std::wstring libraryName(
        forwarder.begin(),
        forwarder.begin() + static_cast<std::ptrdiff_t>(dotPos)
    );

    std::string procedureName(
        forwarder.begin() + static_cast<std::ptrdiff_t>(dotPos + 1),
        forwarder.end()
    );

    PVOID libraryDllBase = SeLoadLibrary(libraryName.c_str());
    if (!libraryDllBase)
        return ExportAddress;

    if (!procedureName.empty() && procedureName.front() == '#')
    {
        std::string_view ordinalText{ procedureName };
        ordinalText.remove_prefix(1);

        unsigned long long ordinalValue = 0;
        const char* begin = ordinalText.data();
        const char* end = begin + ordinalText.size();

        auto [ptr, ec] = std::from_chars(begin, end, ordinalValue);

#undef max

        if (ec == std::errc{} && ptr == end && ordinalValue <= std::numeric_limits<USHORT>::max())
        {
            ExportAddress = SeGetDllBaseProcedureAddress(
                libraryDllBase,
                nullptr,
                static_cast<USHORT>(ordinalValue)
            );
        }
    }
    else
    {
        ExportAddress = SeGetDllBaseProcedureAddress(
            libraryDllBase,
            procedureName.c_str(),
            0
        );
    }

    return ExportAddress;
}

PVOID SeGetLoaderEntryImageExportFunction(
    _In_ PVOID BaseAddress,
    _In_ PIMAGE_NT_HEADERS ImageNtHeader,
    _In_ PIMAGE_DATA_DIRECTORY DataDirectory,
    _In_ PIMAGE_EXPORT_DIRECTORY ExportDirectory,
    _In_opt_ PCSTR ExportName,
    _In_opt_ USHORT ExportOrdinal
)
{
    PVOID exportAddress = NULL;
    PULONG exportAddressTable;
    PULONG exportNameTable;
    PUSHORT exportOrdinalTable;
    ULONG imageSize;

    if (!SepValidateExportTableRvas(ImageNtHeader, ExportDirectory))
        return NULL;

    imageSize = ImageNtHeader->OptionalHeader.SizeOfImage;
    exportAddressTable = static_cast<PULONG>(PTR_ADD_OFFSET(BaseAddress, ExportDirectory->AddressOfFunctions));
    exportNameTable = static_cast<PULONG>(PTR_ADD_OFFSET(BaseAddress, ExportDirectory->AddressOfNames));
    exportOrdinalTable = static_cast<PUSHORT>(PTR_ADD_OFFSET(BaseAddress, ExportDirectory->AddressOfNameOrdinals));

    if (ExportOrdinal)
    {
        ULONG maxOrdinal;
        ULONG functionRva;

        if (ExportOrdinal < ExportDirectory->Base)
            return NULL;
        if (!NT_SUCCESS(RtlULongAdd(ExportDirectory->Base, ExportDirectory->NumberOfFunctions, &maxOrdinal)))
            return NULL;

        if (ExportOrdinal > maxOrdinal)
            return NULL;

        functionRva = exportAddressTable[ExportOrdinal - ExportDirectory->Base];

        if (functionRva == 0 || functionRva >= imageSize)
        {
            return NULL;
        }

        exportAddress = PTR_ADD_OFFSET(BaseAddress, functionRva);
    }
    else if (ExportName)
    {
        ULONG exportIndex;
        USHORT ordinalIndex;
        ULONG functionRva;

        exportIndex = SepLookupLoaderEntryImageExportFunctionIndex(
            BaseAddress,
            ImageNtHeader,
            ExportDirectory,
            exportNameTable,
            ExportName
        );

        if (exportIndex == ULONG_MAX)
            return NULL;

        ordinalIndex = exportOrdinalTable[exportIndex];

        if (ordinalIndex >= ExportDirectory->NumberOfFunctions)
            return NULL;

        functionRva = exportAddressTable[ordinalIndex];
        if (functionRva == 0 || functionRva >= imageSize)
            return NULL;

        exportAddress = PTR_ADD_OFFSET(BaseAddress, functionRva);
    }

    if (!exportAddress)
        return NULL;

    return SepResolveExportForwarder(BaseAddress, exportAddress, DataDirectory, ExportDirectory);
}

NTSTATUS SeGetLoaderEntryImageDirectory(
    _In_ PVOID BaseAddress,
    _In_ PIMAGE_NT_HEADERS ImageNtHeader,
    _In_ ULONG ImageDirectoryIndex,
    _Out_ PIMAGE_DATA_DIRECTORY* ImageDataDirectoryEntry,
    _Out_ PVOID* ImageDirectoryEntry,
    _Out_opt_ SIZE_T* ImageDirectoryLength
)
{
    PIMAGE_DATA_DIRECTORY directory;

    directory = &ImageNtHeader->OptionalHeader.DataDirectory[ImageDirectoryIndex];

    if (directory->VirtualAddress == 0 || directory->Size == 0)
        return STATUS_INVALID_FILE_FOR_SECTION;

    *ImageDataDirectoryEntry = directory;
    *ImageDirectoryEntry = PTR_ADD_OFFSET(BaseAddress, directory->VirtualAddress);
    if (ImageDirectoryLength) *ImageDirectoryLength = directory->Size;

    return STATUS_SUCCESS;
}

NTSTATUS SeGetLoaderEntryImageNtHeaders(
    _In_ PVOID BaseAddress,
    _Out_ PIMAGE_NT_HEADERS* ImageNtHeaders
)
{
    PIMAGE_DOS_HEADER imageDosHeader;
    PIMAGE_NT_HEADERS imageNtHeaders;
    ULONG imageNtHeadersOffset;

    imageDosHeader = static_cast<PIMAGE_DOS_HEADER>(PTR_ADD_OFFSET(BaseAddress, 0));

    if (imageDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
        return STATUS_INVALID_IMAGE_NOT_MZ;

    imageNtHeadersOffset = (ULONG)imageDosHeader->e_lfanew;

    if (imageNtHeadersOffset == 0 || imageNtHeadersOffset >= RTL_IMAGE_MAX_DOS_HEADER)
        return STATUS_INVALID_IMAGE_FORMAT;

    imageNtHeaders = static_cast<PIMAGE_NT_HEADERS>(PTR_ADD_OFFSET(BaseAddress, imageNtHeadersOffset));

    if (imageNtHeaders->Signature != IMAGE_NT_SIGNATURE)
        return STATUS_INVALID_IMAGE_FORMAT;

    *ImageNtHeaders = imageNtHeaders;
    return STATUS_SUCCESS;
}

BOOLEAN SeLoaderEntryImageExportSupressionPresent(
    _In_ PVOID BaseAddress,
    _In_ PIMAGE_NT_HEADERS ImageNtHeader
)
{
    PIMAGE_LOAD_CONFIG_DIRECTORY configDirectory;
    PIMAGE_DATA_DIRECTORY dataDirectory;

    if (NT_SUCCESS(SeGetLoaderEntryImageDirectory(
        BaseAddress,
        ImageNtHeader,
        IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG,
        &dataDirectory,
        (PVOID*)&configDirectory,
        NULL
    )))
    {
        if (RTL_CONTAINS_FIELD(configDirectory, configDirectory->Size, GuardFlags))
        {
            if (BooleanFlagOn(configDirectory->GuardFlags, IMAGE_GUARD_CF_EXPORT_SUPPRESSION_INFO_PRESENT))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

VOID SeLoaderEntryGrantSuppressedCall(
    _In_ PVOID ExportAddress
)
{
    static std::unordered_set<PVOID> cache;
    static std::mutex cacheMutex;

    {
        std::lock_guard<std::mutex> lock(cacheMutex);

        if (cache.contains(ExportAddress))
            return;
    }

    if (NT_SUCCESS(SeGuardGrantSuppressedCallAccess(NtCurrentProcess(), ExportAddress)))
    {
        std::lock_guard<std::mutex> lock(cacheMutex);
        cache.insert(ExportAddress);
    }
}

PVOID SeGetDllBaseProcedureAddress(
    _In_ PVOID DllBase,
    _In_opt_ PCSTR ProcedureName,
    _In_opt_ USHORT ProcedureNumber
)
{
    static SE_INITONCE initOnce = SE_INITONCE_INIT;
    static BOOLEAN exportSuppressionEnabled = FALSE;

    PVOID exportAddress;
    PIMAGE_NT_HEADERS imageNtHeader;
    PIMAGE_DATA_DIRECTORY dataDirectory;
    PIMAGE_EXPORT_DIRECTORY exportDirectory;
    PROCESS_MITIGATION_POLICY_INFORMATION mitigation;

    if (SeBeginInitOnce(&initOnce))
    {
        if (NT_SUCCESS(SeGetProcessMitigationPolicy(NtCurrentProcess(), ProcessControlFlowGuardPolicy, &mitigation)) &&
            mitigation.ControlFlowGuardPolicy.EnableExportSuppression)
        {
            exportSuppressionEnabled = TRUE;
        }

        SeEndInitOnce(&initOnce);
    }

    if (!NT_SUCCESS(SepGetExportDirectory(
        DllBase,
        &imageNtHeader,
        &dataDirectory,
        &exportDirectory
    )))
    {
        return NULL;
    }

    exportAddress = SeGetLoaderEntryImageExportFunction(
        DllBase,
        imageNtHeader,
        dataDirectory,
        exportDirectory,
        ProcedureName,
        ProcedureNumber
    );

    if (exportAddress && exportSuppressionEnabled)
    {
        if (SeLoaderEntryImageExportSupressionPresent(DllBase, imageNtHeader))
        {
            SeLoaderEntryGrantSuppressedCall(exportAddress);
        }
    }

    return exportAddress;
}

PVOID SeGetDllBaseProcedureAddressZ(
    _In_ PCWSTR DllName,
    _In_opt_ PCSTR ProcedureName,
    _In_opt_ USHORT ProcedureNumber
)
{
    PVOID baseAddress;

    if (!(baseAddress = SeGetLoaderEntryDllBase(NULL, DllName)))
        return NULL;

    return SeGetDllBaseProcedureAddress(
        baseAddress,
        ProcedureName,
        ProcedureNumber
    );
}

PVOID SeLoadLibrary(
    _In_ PCWSTR FileName
)
{
    PVOID baseAddress;

    if (baseAddress = LoadLibraryEx(FileName, NULL, LOAD_LIBRARY_SEARCH_SYSTEM32))
        return baseAddress;

    if (baseAddress = LoadLibraryEx(FileName, NULL, LOAD_LIBRARY_SEARCH_APPLICATION_DIR))
        return baseAddress;

    return NULL;
}

PLDR_DATA_TABLE_ENTRY SeFindLoaderEntryNameHash(
    _In_ ULONG BaseNameHash
)
{
    PLDR_DATA_TABLE_ENTRY entry;
    PLIST_ENTRY listHead;
    PLIST_ENTRY listEntry;

    listHead = &NtCurrentPeb()->Ldr->InLoadOrderModuleList;
    listEntry = listHead->Flink;

    while (listEntry != listHead)
    {
        entry = CONTAINING_RECORD(listEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

        if (entry->BaseNameHashValue == BaseNameHash)
            return entry;

        listEntry = listEntry->Flink;
    }

    return NULL;
}

_Use_decl_annotations_
PVOID SeGetLoaderEntryDllBase(
    _In_opt_ PCWSTR FullDllName,
    _In_opt_ PCWSTR BaseDllName
)
{
    PLDR_DATA_TABLE_ENTRY entry;
    PVOID baseAddress = NULL;

    if (!BaseDllName)
        return NULL;

    SeAcquireLoaderLock();

    UNICODE_STRING uBaseName;
    RtlInitUnicodeString(&uBaseName, BaseDllName);

    ULONG baseNameHash{ 0 };
    if (NT_SUCCESS(RtlHashUnicodeString(&uBaseName, TRUE, HASH_STRING_ALGORITHM_DEFAULT, &baseNameHash)))
    {
        entry = SeFindLoaderEntryNameHash(baseNameHash);
        if (entry)
            baseAddress = entry->DllBase;
    }

    SeReleaseLoaderLock();

    return baseAddress;
}

NTSTATUS SeLoadResource(
    _In_ PVOID DllBase,
    _In_ PCWSTR Name,
    _In_ PCWSTR Type,
    _Out_opt_ ULONG* ResourceLength,
    _Out_opt_ PVOID* ResourceBuffer
)
{
    NTSTATUS status;
    PIMAGE_RESOURCE_DATA_ENTRY resourceData = NULL;
    PVOID resourceBuffer = NULL;
    ULONG resourceLength;
    ULONG_PTR resourcePath[] = {
        (ULONG_PTR)Type,
        (ULONG_PTR)Name,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL)
    };

    __try
    {
        //status = LdrFindResource_U(DllBase, (PLDR_RESOURCE_INFO)resourcePath, RTL_NUMBER_OF(resourcePath), &resourceData);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        status = GetExceptionCode();
    }

    if (!NT_SUCCESS(status))
        return status;

    __try
    {
        status = LdrAccessResource(DllBase, resourceData, &resourceBuffer, &resourceLength);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        status = GetExceptionCode();
    }

    if (!NT_SUCCESS(status))
        return status;

    if (ResourceLength)
        *ResourceLength = resourceLength;
    if (ResourceBuffer)
        *ResourceBuffer = resourceBuffer;

    return status;
}

NTSTATUS SeLoadResourceCopy(
    _In_ PVOID DllBase,
    _In_ PCWSTR Name,
    _In_ PCWSTR Type,
    _Out_opt_ ULONG* ResourceLength,
    _Out_opt_ PVOID* ResourceBuffer
)
{
    NTSTATUS status;
    ULONG resourceLength;
    PVOID resourceBuffer;

    status = SeLoadResource(
        DllBase,
        Name,
        Type,
        &resourceLength,
        &resourceBuffer
    );

    if (NT_SUCCESS(status))
    {
        if (ResourceLength)
            *ResourceLength = resourceLength;
        if (ResourceBuffer)
            *ResourceBuffer = SeAllocateCopy(resourceBuffer, resourceLength);
    }

    return status;
}