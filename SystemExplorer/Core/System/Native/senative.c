#include "senative.h"
#include "apiimport.h"

//DECLSPEC_GUARDNOCF bilya cho ni robit
static NTSTATUS SepSetInformationVirtualMemory(
    _In_ HANDLE ProcessHandle,
    _In_ VIRTUAL_MEMORY_INFORMATION_CLASS VmInformationClass,
    _In_ ULONG_PTR NumberOfEntries,
    _In_reads_(NumberOfEntries) PMEMORY_RANGE_ENTRY VirtualAddresses,
    _In_reads_bytes_(VmInformationLength) PVOID VmInformation,
    _In_ ULONG VmInformationLength
)
{
    /*return NtSetInformationVirtualMemory_Import()(
        ProcessHandle,
        VmInformationClass,
        NumberOfEntries,
        VirtualAddresses,
        VmInformation,
        VmInformationLength
    );*/
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS SeGuardGrantSuppressedCallAccess(
    _In_ HANDLE ProcessHandle,
    _In_ PVOID VirtualAddress
)
{
    NTSTATUS status;
    MEMORY_RANGE_ENTRY cfgCallTargetRangeInfo;
    CFG_CALL_TARGET_INFO cfgCallTargetInfo;
    CFG_CALL_TARGET_LIST_INFORMATION cfgCallTargetListInfo;
    ULONG numberOfEntriesProcessed = 0;

  /*  if (!NtSetInformationVirtualMemory_Import())
        return STATUS_PROCEDURE_NOT_FOUND;*/

    memset(&cfgCallTargetRangeInfo, 0, sizeof(MEMORY_RANGE_ENTRY));
    cfgCallTargetRangeInfo.VirtualAddress = PAGE_ALIGN(VirtualAddress);
    cfgCallTargetRangeInfo.NumberOfBytes = PAGE_SIZE;

    memset(&cfgCallTargetInfo, 0, sizeof(CFG_CALL_TARGET_INFO));
    cfgCallTargetInfo.Offset = BYTE_OFFSET(VirtualAddress);
    cfgCallTargetInfo.Flags = CFG_CALL_TARGET_VALID;

    memset(&cfgCallTargetListInfo, 0, sizeof(CFG_CALL_TARGET_LIST_INFORMATION));
    cfgCallTargetListInfo.NumberOfEntries = 1;
    cfgCallTargetListInfo.Reserved = 0;
    cfgCallTargetListInfo.NumberOfEntriesProcessed = &numberOfEntriesProcessed;
    cfgCallTargetListInfo.CallTargetInfo = &cfgCallTargetInfo;

    status = SepSetInformationVirtualMemory(
        ProcessHandle,
        VmCfgCallTargetInformation,
        1,
        &cfgCallTargetRangeInfo,
        &cfgCallTargetListInfo,
        sizeof(CFG_CALL_TARGET_LIST_INFORMATION)
    );

    if (status == STATUS_INVALID_PAGE_PROTECTION)
        status = STATUS_SUCCESS;

    return status;
}
