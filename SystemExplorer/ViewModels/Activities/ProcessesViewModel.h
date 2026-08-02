#pragma once

#include "../ViewModelBase.h"
#include "ViewModels/Activities/ProcessesViewModel.g.h"

#include <Core/Data/Items/ProcessItem.h>
#include "../ViewModelBase.h"

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::Storage;
    using namespace winrt::Windows::System;
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Microsoft::UI::Xaml;

    using namespace winrt::SystemExplorer::Core;
    using namespace winrt::SystemExplorer::Core::Data::Items;
    using namespace winrt::SystemExplorer::Xaml;
}

namespace winrt::SystemExplorer::ViewModels::Activities::implementation
{
    struct ProcessesViewModel : ProcessesViewModelT<ProcessesViewModel, ::winrt::SystemExplorer::ViewModels::implementation::ViewModelBase>
    {
        ProcessesViewModel();

        wil::single_threaded_property<winrt::IObservableVector<winrt::ProcessItem>> Processes = winrt::single_threaded_observable_vector<winrt::ProcessItem>();

        wil::single_threaded_property<winrt::IAsyncRelayCommand> TerminateProcessCommand = winrt::AsyncRelayCommandFactory::Make([this](auto&&) -> winrt::IAsyncAction {
            co_await doTerminateProcessAsync();
        }, [this](auto&&) -> bool {
                return SelectedProcess_ != nullptr;
        });

        wil::single_threaded_property<winrt::IAsyncRelayCommand> EfficiencyModeCommand = winrt::AsyncRelayCommandFactory::Make([this](auto&&) -> winrt::IAsyncAction {
            co_await doSetEfficiencyModeAsync();
        }, [this](auto&&) -> bool {
            return SelectedProcess_ != nullptr;
        });

        wil::single_threaded_property<winrt::IAsyncRelayCommand> RestartProcessCommand = winrt::AsyncRelayCommandFactory::Make([this](auto&&) -> winrt::IAsyncAction {
            co_await doRestartProcessAsync();
        });

        wil::single_threaded_property<winrt::IAsyncRelayCommand> OpenProcessDetailsWindowCommand = winrt::AsyncRelayCommandFactory::Make([this](auto&&) -> winrt::IAsyncAction {
            co_await doOpenProcessDetailsWindowAsync();
        });

        wil::single_threaded_property<winrt::IAsyncRelayCommand> SuspendProcessCommand = winrt::AsyncRelayCommandFactory::Make([this](auto&&) -> winrt::IAsyncAction {
            co_return;
        });

        wil::single_threaded_property<winrt::IAsyncRelayCommand> OpenFileLocationCommand = winrt::AsyncRelayCommandFactory::Make([this](auto&&) -> winrt::IAsyncAction {
            co_await doOpenProcessLocationAsync();
        });

        wil::single_threaded_property<winrt::IAsyncRelayCommand> SearchOnlineCommand = winrt::AsyncRelayCommandFactory::Make([this](auto&&) -> winrt::IAsyncAction {
            co_await Launcher::LaunchUriAsync(Uri{ L"https://www.bing.com/search?q=" + SelectedProcess_.Name() });
        });

        wil::single_threaded_property<winrt::IAsyncRelayCommand> DumpProcessMemoryCommand = winrt::AsyncRelayCommandFactory::Make([this](auto const& parameter) -> winrt::IAsyncAction {
           /* auto dumpType = parameter.as<int32_t>();
            co_await doDumpProcessMemoryAsync(static_cast<MINIDUMP_TYPE>(dumpType));*/
            co_return;
        });

        wil::single_threaded_property<winrt::IAsyncRelayCommand> ChangeItemSizeCommand = winrt::AsyncRelayCommandFactory::Make([this](auto const& parameter) -> winrt::IAsyncAction {
            /*auto newSize = parameter.as<SystemExplorer::Core::Data::Enums::ItemSize>();

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
            }*/
            co_return;
        });

        DECLARE_ONLY_SETTER(winrt::ProcessItem, SelectedProcess, nullptr);


        WIL_NOTIFYING_PROPERTY(float, TotalCpuUsage, 0);
        WIL_NOTIFYING_PROPERTY(uint64_t, TotalIoRate, 0);
        WIL_NOTIFYING_PROPERTY(uint64_t, TotalPrivateBytes, 0);
        WIL_NOTIFYING_PROPERTY(int32_t, ItemIconSize, 18);
        WIL_NOTIFYING_PROPERTY(int32_t, ItemFontSize, 13);
    private: // internal

        [[nodiscard]] winrt::IAsyncAction showErrorDialogAsync(hstring const& message);
    private: // commands
        [[nodiscard]] winrt::IAsyncAction doTerminateProcessAsync();
        [[nodiscard]] winrt::IAsyncAction doSetEfficiencyModeAsync();
        [[nodiscard]] winrt::IAsyncAction doRestartProcessAsync();
        [[nodiscard]] winrt::IAsyncAction doOpenProcessDetailsWindowAsync();
        [[nodiscard]] winrt::IAsyncAction doOpenProcessLocationAsync();
       /* winrt::IAsyncAction doDumpProcessMemoryAsync(MINIDUMP_TYPE dumpType);*/
    };
}
FACTORY(winrt::SystemExplorer::ViewModels::Activities, ProcessesViewModel);