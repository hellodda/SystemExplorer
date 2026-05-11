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