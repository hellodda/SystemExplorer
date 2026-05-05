#pragma once
#include "ProviderThread.h"
#include <winrt/SystemExplorer.Models.h>
#include <Helpers/Win32Helper.h>

namespace winrt::SystemExplorer::Core::System
{
    enum class ProcessStatus : DWORD
    {
        Running,
        Suspended,
        NotResponding,
        Terminated
    };

    struct ProcessNativeInformation
    {
        uint32_t Pid;
        uint32_t ParentId;
        uint32_t IoRate;
        uint32_t PrivateBytes;
        float CpuUsage;

        ProcessStatus Status;
        bool IsEfficiencyModeEnabled;

        std::wstring Name;
        std::wstring Description;
        HICON Icon;
    };
}

namespace winrt::SystemExplorer::Core::System::Contracts
{
    using namespace winrt::SystemExplorer::Models;

    __interface IInformationMonitor
    {
         ProviderThread& Thread();
    };

    __interface IProcessManager
    {
        void Terminate(uint32_t pid);
        void Restart(uint32_t pid);
        void EnableEfficiencyMode(uint32_t pid);
        void DisableEfficiencyMode(uint32_t pid);
    };

    __interface IProcessInformationProvider : IInformationMonitor
    {
        std::vector<ProcessNativeInformation> GetAllProcesses();
    };

    __interface IServiceInformationProvider
    {

    };
}

