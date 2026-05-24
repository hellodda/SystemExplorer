#pragma once
#include "se.h"
#include "procproc.h"

typedef union _SE_LIVE_DUMP_OPTIONS
{
    BOOLEAN Flags;
    struct
    {
        BOOLEAN CompressMemoryPages : 1;
        BOOLEAN IncludeUserSpaceMemory : 1;
        BOOLEAN IncludeHypervisorPages : 1;
        BOOLEAN OnlyKernelThreadStacks : 1;
        BOOLEAN UseDumpStorageStack : 1;
        BOOLEAN IncludeNonEssentialHypervisorPages : 1;
        BOOLEAN Spare : 2;
    };
} SE_LIVE_DUMP_OPTIONS, * PSE_LIVE_DUMP_OPTIONS;

typedef struct _SE_LIVE_DUMP_CONFIG
{
    PWSTR FileName;
    NTSTATUS LastStatus;
    BOOLEAN KernelDumpActive;
    SE_LIVE_DUMP_OPTIONS Options;
    HANDLE FileHandle;
    HANDLE EventHandle;
} SE_LIVE_DUMP_CONFIG, * PSE_LIVE_DUMP_CONFIG;

typedef struct _SE_PROCESS_MINIDUMP_CONTEXT
{
    HWND WindowHandle;
    HWND ParentWindowHandle;

    HANDLE ProcessId;
    PSE_PROCESS_ITEM ProcessItem;
    PWSTR FileName;
    PWSTR ErrorMessage;
    ULONG DumpType;

    HANDLE ProcessHandle;
    HANDLE FileHandle;
    HANDLE KernelFileHandle;

    union
    {
        BOOLEAN Flags;
        struct
        {
            BOOLEAN IsWow64Process : 1;
            BOOLEAN IsProcessSnapshot : 1;
            BOOLEAN Stop : 1;
            BOOLEAN Succeeded : 1;
            BOOLEAN EnableProcessSnapshot : 1;
            BOOLEAN EnableKernelSnapshot : 1;
            BOOLEAN Spare : 2;
        };
    };

    ULONG64 LastTickCount;
    WNDPROC DefaultTaskDialogWindowProc;
} SE_PROCESS_MINIDUMP_CONTEXT, * PSE_PROCESS_MINIDUMP_CONTEXT;


#ifdef __cplusplus
extern "C" {
#endif

_Function_class_(USER_THREAD_START_ROUTINE)
NTSTATUS SepCreateLiveKernelDump(
    _In_ PSE_LIVE_DUMP_CONFIG Config
);

VOID SeCreateDumpFileProcess(
    _In_ PCWSTR FileName,
    _In_ PSE_PROCESS_ITEM ProcessItem,
    _In_ MINIDUMP_TYPE DumpType
);

#ifdef __cplusplus
}
#endif
