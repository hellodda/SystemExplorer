#pragma once

#include "../ViewModelBase.h"
#include <winrt/SystemExplorer.Xaml.Mvvm.Input.h>
#include "ViewModels/Activities/ProcessesViewModel.g.h"
#include <Core/Settings/UserSettings.h>
#include <Core/System/System.h>
#include <Helpers/Common.h>
#include <factory.h>
#include <property.h>

namespace winrt::SystemExplorer::ViewModels::Activities::implementation
{
    using namespace winrt::Windows::Foundation;
	using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Microsoft::UI::Xaml;

	using namespace winrt::SystemExplorer::Models;
    using namespace winrt::SystemExplorer::Core;
    using namespace winrt::SystemExplorer::Core::System;
    using namespace winrt::SystemExplorer::Core::System::Contracts;
    using namespace winrt::SystemExplorer::Xaml::Mvvm::Input;

    struct ProcessesViewModel : ProcessesViewModelT<ProcessesViewModel, ::winrt::SystemExplorer::ViewModels::implementation::ViewModelBase>
    {
        ProcessesViewModel();

        wil::single_threaded_property<IObservableVector<ProcessInformation>> Processes = single_threaded_observable_vector<ProcessInformation>();
        wil::single_threaded_property<IAsyncRelayCommand> TerminateProcessCommand = AsyncRelayCommandFactory::Make([this](auto&&) -> IAsyncAction {
            co_await doTerminateProcessAsync();
        }, [this](auto&&) -> bool {
            return SelectedProcess_ != nullptr;
        });
        wil::single_threaded_property<IAsyncRelayCommand> EfficiencyModeCommand = AsyncRelayCommandFactory::Make([this](auto&&) -> IAsyncAction {
            co_await doSetEfficiencyModeAsync();
        }, [this](auto&&) -> bool {
            return SelectedProcess_ != nullptr;
        });

        DECLARE_ONLY_SETTER(ProcessInformation, SelectedProcess, nullptr);
        WIL_NOTIFYING_PROPERTY(float, TotalCpuUsage, 0);
        WIL_NOTIFYING_PROPERTY(uint64_t, TotalIoRate, 0);
        WIL_NOTIFYING_PROPERTY(uint64_t, TotalPrivateBytes, 0);
    private: // internal
        void updateProcessesList(std::vector<PROCESS_INFORMATION>& newProcesses);
    private: // commands
        IAsyncAction doTerminateProcessAsync();
        IAsyncAction doSetEfficiencyModeAsync();
    private:
        std::shared_ptr<IProcessInformationProvider> provider_{ nullptr };
        std::shared_ptr<IProcessManager> manager_{ nullptr };
        std::unordered_map<uint32_t, ProcessInformation> uiCache_;
        DispatcherTimer pullTimer_;
    };
}
FACTORY(winrt::SystemExplorer::ViewModels::Activities, ProcessesViewModel);