#include "mapldr.h"
#include "sebasesup.h"

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
        status = LdrFindResource_U(DllBase, resourcePath, RTL_NUMBER_OF(resourcePath), &resourceData);
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