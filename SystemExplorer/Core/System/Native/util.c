#include "util.h"
#include "error.h"
#include "mapldr.h"

NTSTATUS SeGetFileVersionInfo(
    _In_ PCWSTR FileName,
    _Out_ PVOID* VersionInfo
)
{
    NTSTATUS status;
    PVOID libraryModule;
    PVOID versionInfo;

    libraryModule = LoadLibraryEx(
        FileName,
        NULL,
        LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE
    );

    if (!libraryModule)
    {
        return SeGetLastWin32ErrorAsNtStatus();
    }

    status = SeLoadResourceCopy(
        libraryModule,
        MAKEINTRESOURCE(VS_VERSION_INFO),
        VS_FILE_INFO,
        NULL,
        &versionInfo
    );

    if (NT_SUCCESS(status))
    {
        if (SeIsFileVersionInfo32(versionInfo))
        {
            FreeLibrary(libraryModule);
            *VersionInfo = versionInfo;
            return STATUS_SUCCESS;
        }
        else
        {
            status = STATUS_NOT_FOUND;
        }

        SeFree(versionInfo);
    }

    FreeLibrary(libraryModule);

    return status;
}

_Success_(return)
BOOLEAN SeShowTaskDialog(
    _In_ const TASKDIALOGCONFIG * Config,
    _Out_opt_ PULONG Button,
    _Out_opt_ PULONG RadioButton,
    _Out_opt_ PBOOLEAN FlagChecked
)
{
    HRESULT status;
    LONG button;
    LONG radio;
    BOOL selected;

    status = TaskDialogIndirect(
        Config,
        &button,
        &radio,
        &selected
    );

    if (HR_SUCCESS(status))
    {
        if (Button) *Button = button;
        if (RadioButton) *RadioButton = radio;
        if (FlagChecked) *FlagChecked = !!selected;
        return TRUE;
    }

    return FALSE; 
}

LCID SeGetSystemDefaultLCID(
    VOID
)
{
    LCID localeId = LOCALE_SYSTEM_DEFAULT;

    if (NT_SUCCESS(NtQueryDefaultLocale(FALSE, &localeId)))
        return localeId;

    return MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT);
}

LANGID SeGetSystemDefaultLangID(
    VOID
)
{
    return LANGIDFROMLCID(SeGetSystemDefaultLCID());
}
