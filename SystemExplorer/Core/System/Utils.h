#pragma once
#include "System.h"

namespace winrt::SystemExplorer::Core::System::Utils
{
    bool RestartProcess(
        _In_ HANDLE Process
    );
    std::wstring ExtractProcessName(
        _In_ PSYSTEM_PROCESS_INFORMATION pInfo,
        IN uint32_t pid
    );
    bool IsProcessSuspended(
        _In_ PSYSTEM_PROCESS_INFORMATION pInfo
    );

    inline ProcessStatus GetProcessStatus(
        _In_ PSYSTEM_PROCESS_INFORMATION pInfo
    )
    {
        return IsProcessSuspended(pInfo) ? ProcessStatus::Suspended : ProcessStatus::Running;
    }

    inline bool is_valid(
        _In_ HANDLE handle
    )
    {
        return handle != INVALID_HANDLE_VALUE;
    }

    // ntqueryinfo izi



}

