#include "pch.h"
#include "System.h"

bool IsEfficiencyModeEnabledByPid(uint32_t pid)
{
    if (pid <= 4) return false;

    wil::unique_process_handle handle{ OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid) };
    
    if (!handle.is_valid())
        return false;
    
    PROCESS_POWER_THROTTLING_STATE state{};
    state.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;

    bool isEfficiency = false;
    if (GetProcessInformation(handle.get(), ProcessPowerThrottling, &state, sizeof(state)))
    {
        isEfficiency = (state.ControlMask & PROCESS_POWER_THROTTLING_EXECUTION_SPEED) &&
            (state.StateMask & PROCESS_POWER_THROTTLING_EXECUTION_SPEED);
    }
    return isEfficiency;
}
