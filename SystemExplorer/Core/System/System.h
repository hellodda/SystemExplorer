#pragma once
#include <winrt/SystemExplorer.Models.h>
#include <Helpers/Win32Helper.h>

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
        std::vector<ProcessInformation> GetAllProcesses();

        IProviderThread* Thread();
    };

    __interface IServiceInformationProvider
    {

    };
}

namespace winrt::SystemExplorer::Core::System
{

}