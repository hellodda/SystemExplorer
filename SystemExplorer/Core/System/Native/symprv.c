#include "symprv.h"
#include "error.h"
#pragma comment(lib, "Dbghelp.lib")

HRESULT SeWriteMiniDumpProcess(
    _In_ HANDLE ProcessHandle,
    _In_ HANDLE ProcessId,
    _In_ HANDLE FileHandle,
    _In_ MINIDUMP_TYPE DumpType,
    _In_opt_ PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
    _In_opt_ PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
    _In_opt_ PMINIDUMP_CALLBACK_INFORMATION CallbackParam
)
{
    if (!MiniDumpWriteDump)
    {
        return HRESULT_FROM_WIN32(ERROR_PROC_NOT_FOUND);
    }

    if (!MiniDumpWriteDump(
        ProcessHandle,
        HandleToUlong(ProcessId),
        FileHandle,
        DumpType,
        ExceptionParam,
        UserStreamParam,
        CallbackParam
    ))
    {
        return (HRESULT)SeGetLastError();
    }

    return S_OK;
}