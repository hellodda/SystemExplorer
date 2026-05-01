#pragma once
#include <winrt/SystemExplorer.Models.h>
#include <Helpers/Win32Helper.h>

namespace winrt::SystemExplorer::Core::System
{
    typedef struct PROCESS_INFORMATION
    {
        uint32_t Pid;
        uint32_t ParentId;
        uint32_t IoRate;
        uint32_t PrivateBytes;
        float CpuUsage;

        const wchar_t* Name;
        const wchar_t* Description;
    } PPROSESS_INFORMATION;
    typedef struct SERVICE_INFORMATION
    {

    } PSERVICE_INFORMATION;
    typedef struct HANDLE_INFORMATION
    {

    };
    typedef struct OBJECT_INFORMATION
    {

    };
}

namespace winrt::SystemExplorer::Core::System::Contracts
{
    using namespace winrt::SystemExplorer::Models;

    __interface IProviderThread
    {
        void SetInterval(std::chrono::milliseconds interval);
        void Resume();
        void Suspend();
    };

    __interface IProcessInformationProvider
    {
        std::vector<PROCESS_INFORMATION> GetAllProcesses();

        IProviderThread* Thread();
    };

    __interface IServiceInformationProvider
    {

    };
}
