#include "dump.h"
#include "process.h"
#include "symprv.h"

_Function_class_(USER_THREAD_START_ROUTINE)
NTSTATUS SepCreateLiveKernelDump(
    _In_ PSE_LIVE_DUMP_CONFIG Config
)
{
    NTSTATUS status;
    SYSDBG_LIVEDUMP_CONTROL liveDumpControl;
    SYSDBG_LIVEDUMP_CONTROL_FLAGS flags;
    SYSDBG_LIVEDUMP_CONTROL_ADDPAGES pages;
    SYSDBG_LIVEDUMP_SELECTIVE_CONTROL selective;
    ULONG length;

    memset(&liveDumpControl, 0, sizeof(SYSDBG_LIVEDUMP_CONTROL));
    memset(&flags, 0, sizeof(SYSDBG_LIVEDUMP_CONTROL_FLAGS));
    memset(&pages, 0, sizeof(SYSDBG_LIVEDUMP_CONTROL_ADDPAGES));
    memset(&selective, 0, sizeof(SYSDBG_LIVEDUMP_SELECTIVE_CONTROL));

    if (Config->Options.UseDumpStorageStack)
        flags.UseDumpStorageStack = TRUE;
    if (Config->Options.CompressMemoryPages)
        flags.CompressMemoryPagesData = TRUE;
    if (Config->Options.IncludeUserSpaceMemory)
        flags.IncludeUserSpaceMemoryPages = TRUE;
    if (Config->Options.IncludeHypervisorPages)
        pages.HypervisorPages = TRUE;
    if (Config->Options.IncludeNonEssentialHypervisorPages)
        pages.NonEssentialHypervisorPages = TRUE;

    if (Config->Options.OnlyKernelThreadStacks)
    {
        liveDumpControl.Version = SYSDBG_LIVEDUMP_CONTROL_VERSION_2;
        flags.SelectiveDump = TRUE;
        length = sizeof(SYSDBG_LIVEDUMP_CONTROL);

        selective.Version = SYSDBG_LIVEDUMP_SELECTIVE_CONTROL_VERSION;
        selective.Size = sizeof(SYSDBG_LIVEDUMP_SELECTIVE_CONTROL);
        selective.ThreadKernelStacks = TRUE;

        liveDumpControl.SelectiveControl = &selective;
    }
    else
    {
        liveDumpControl.Version = SYSDBG_LIVEDUMP_CONTROL_VERSION_1;
        length = sizeof(SYSDBG_LIVEDUMP_CONTROL_V1);
    }

    liveDumpControl.DumpFileHandle = Config->FileHandle;
    liveDumpControl.CancelEventHandle = Config->EventHandle;
    liveDumpControl.Flags = flags;
    liveDumpControl.AddPagesControl = pages;

    status = NtSystemDebugControl(
        SysDbgGetLiveKernelDump,
        &liveDumpControl,
        length,
        NULL,
        0,
        NULL
    );

    Config->LastStatus = status;
    Config->KernelDumpActive = FALSE;

    return status;
}

VOID SeCreateDumpFileProcess(
    _In_ PCWSTR FileName,
    _In_ PSE_PROCESS_ITEM ProcessItem,
    _In_ MINIDUMP_TYPE DumpType
)
{
    if (!FileName || !ProcessItem)
        return;

    static ACCESS_MASK processAccess[] =
    {
        PROCESS_ALL_ACCESS,
        PROCESS_QUERY_INFORMATION | PROCESS_DUP_HANDLE | PROCESS_VM_READ,
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
        PROCESS_QUERY_INFORMATION
    };

    NTSTATUS status = STATUS_UNSUCCESSFUL;
    HANDLE processHandle = NULL;
    HANDLE fileHandle = INVALID_HANDLE_VALUE;

    for (ULONG i = 0; i < RTL_NUMBER_OF(processAccess); i++)
    {
        status = SeOpenProcess(&processHandle, processAccess[i], ProcessItem->ProcessId);
        if (NT_SUCCESS(status))
        {
            break;
        }
    }

    if (!NT_SUCCESS(status))
    {
        return;
    }

    fileHandle = CreateFileW(
        FileName,
        GENERIC_WRITE,          
        0,                       
        NULL,                   
        CREATE_ALWAYS,            
        FILE_ATTRIBUTE_NORMAL,   
        NULL                     
    );

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        CloseHandle(processHandle); 
        return;
    }

    SeWriteMiniDumpProcess(
        processHandle,
        ProcessItem->ProcessId,
        fileHandle,
        DumpType,
        NULL,
        NULL,
        NULL
    );
    CloseHandle(fileHandle);
    CloseHandle(processHandle);
}