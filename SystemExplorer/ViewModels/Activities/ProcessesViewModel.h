#pragma once

#include "../ViewModelBase.h"
#include "ViewModels/Activities/ProcessesViewModel.g.h"
#include <winrt/Windows.System.h>
#include <winrt/Windows.Storage.h>
#include <Core/Data/Items/ProcessItem.h>
#include <Core/Settings/UserSettings.h>
#include <Core/System/System.h>
#include <Core/System/ProcessInformationProvider.h> 
#include <winrt/SystemExplorer.Xaml.Mvvm.Input.h>
#include <Helpers/Common.h>
#include <factory.h>
#include <property.h>
#include <regex>

#include "../../../Common/cache.h"

namespace winrt::SystemExplorer::ViewModels::Activities::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::Storage;
    using namespace winrt::Windows::System;
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Microsoft::UI::Xaml;

    using namespace winrt::SystemExplorer::Core;
    using namespace winrt::SystemExplorer::Core::System;
    using namespace winrt::SystemExplorer::Core::Data::Items;
    using namespace winrt::SystemExplorer::Core::System::Contracts;
    using namespace winrt::SystemExplorer::Xaml::Mvvm::Input;

    struct ProcessesViewModel : ProcessesViewModelT<ProcessesViewModel, ::winrt::SystemExplorer::ViewModels::implementation::ViewModelBase>
    {
        ProcessesViewModel();

        wil::single_threaded_property<IObservableVector<ProcessItem>> Processes = single_threaded_observable_vector<ProcessItem>();

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

        wil::single_threaded_property<IAsyncRelayCommand> RestartProcessCommand = AsyncRelayCommandFactory::Make([this](auto&&) -> IAsyncAction {
            co_await doRestartProcessAsync();
        });

        wil::single_threaded_property<IAsyncRelayCommand> OpenProcessDetailsWindowCommand = AsyncRelayCommandFactory::Make([this](auto&&) -> IAsyncAction {
            co_await doOpenProcessDetailsWindowAsync();
        });

        wil::single_threaded_property<IAsyncRelayCommand> SuspendProcessCommand = AsyncRelayCommandFactory::Make([this](auto&&) -> IAsyncAction {
            co_return;
        });

        wil::single_threaded_property<IAsyncRelayCommand> OpenFileLocationCommand = AsyncRelayCommandFactory::Make([this](auto&&) -> IAsyncAction {
            co_await doOpenProcessLocationAsync();
        });

        wil::single_threaded_property<IAsyncRelayCommand> SearchOnlineCommand = AsyncRelayCommandFactory::Make([this](auto&&) -> IAsyncAction {
            co_await Launcher::LaunchUriAsync(Uri{ L"https://www.bing.com/search?q=" + SelectedProcess_.Name() });
        });

        wil::single_threaded_property<IAsyncRelayCommand> DumpProcessMemoryCommand = AsyncRelayCommandFactory::Make([this](auto const& parameter) -> IAsyncAction {
            auto dumpType = parameter.as<int32_t>();
            co_await doDumpProcessMemoryAsync(static_cast<MINIDUMP_TYPE>(dumpType));
        });

        wil::single_threaded_property<IAsyncRelayCommand> ChangeItemSizeCommand = AsyncRelayCommandFactory::Make([this](auto const& parameter) -> IAsyncAction {
            auto newSize = parameter.as<SystemExplorer::Core::Data::Enums::ItemSize>();

            auto state = SystemExplorer::Core::Settings::AppState::Instance();

            switch (newSize)
            {
            case SystemExplorer::Core::Data::Enums::ItemSize::Large:
            {
                state.ItemIconSize(22);
                state.ItemFontSize(16);
                break;
            }
            case SystemExplorer::Core::Data::Enums::ItemSize::Normal:
            {
                state.ItemFontSize(13);
                state.ItemIconSize(18);
                break;
            }
            case SystemExplorer::Core::Data::Enums::ItemSize::Small:
            {
                state.ItemFontSize(10);
                state.ItemIconSize(15);
                break;
            }
            }
            co_return;
        });

        DECLARE_ONLY_SETTER(hstring, SearchString, L"");
        DECLARE_ONLY_SETTER(ProcessItem, SelectedProcess, nullptr);

        WIL_NOTIFYING_PROPERTY(float, TotalCpuUsage, 0);
        WIL_NOTIFYING_PROPERTY(uint64_t, TotalIoRate, 0);
        WIL_NOTIFYING_PROPERTY(uint64_t, TotalPrivateBytes, 0);
        WIL_NOTIFYING_PROPERTY(int32_t, ItemIconSize, 18);
        WIL_NOTIFYING_PROPERTY(int32_t, ItemFontSize, 13);
    private: // internal
        void applyTransformations();
        void updateProcessesList(std::vector<native::shared_process_item>& newProcesses);
        void updateMetricsAndCache(absl::flat_hash_set<uint32_t>& outActivePids);
        void pruneDeadProcesses(absl::flat_hash_set<uint32_t> const& activePids);

        IAsyncAction showErrorDialogAsync(hstring const& message);
    private: // commands
        IAsyncAction doTerminateProcessAsync();
        IAsyncAction doSetEfficiencyModeAsync();
        IAsyncAction doRestartProcessAsync();
        IAsyncAction doOpenProcessDetailsWindowAsync();
        IAsyncAction doOpenProcessLocationAsync();
        IAsyncAction doDumpProcessMemoryAsync(MINIDUMP_TYPE dumpType);
    private:
        std::shared_ptr<ProcessInformationProvider> provider_{ nullptr };
        std::shared_ptr<IProcessManager> manager_{ nullptr };

        utils::cache_tracker<uint32_t, ProcessItem> itemCache_;
        absl::flat_hash_map<uint32_t, std::chrono::steady_clock::time_point> deadProcesses_;

        DispatcherTimer pullTimer_;

        std::vector<native::shared_process_item> lastRawProcesses_;
        std::optional<std::wregex> searchRegex_;
    };
}
FACTORY(winrt::SystemExplorer::ViewModels::Activities, ProcessesViewModel);