#pragma once
#include "ProviderThread.h"

namespace winrt::SystemExplorer::Core::System::Contracts
{
    __interface IProcessManager
    {
        void Terminate(uint32_t pid);
        void Restart(uint32_t pid);
        void Suspend(uint32_t pid);
        void Resume(uint32_t pid);
        void EnableEfficiencyMode(uint32_t pid);
        void DisableEfficiencyMode(uint32_t pid);
    };

}
