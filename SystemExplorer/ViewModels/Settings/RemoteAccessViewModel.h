#pragma once

#include "ViewModels/Settings/RemoteAccessViewModel.g.h"

#include <Core/Data/Items/RemoteComputerItem.h>
#include <Core/Eil/event.h>

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

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    struct RemoteAccessViewModel : RemoteAccessViewModelT<RemoteAccessViewModel>
    {
        RemoteAccessViewModel();

        wil::single_threaded_rw_property<winrt::RemoteComputerItem> SelectedDevice;

        wil::single_threaded_property<winrt::IObservableVector<winrt::IInspectable>> Devices
            = winrt::single_threaded_observable_vector<winrt::IInspectable>();

        wil::single_threaded_property<winrt::IAsyncRelayCommand> ConnectToDeviceCommand = winrt::AsyncRelayCommandFactory::Make([this](winrt::IInspectable const& parameter) -> winrt::IAsyncAction {
            co_return;
        });

        wil::single_threaded_property<winrt::IAsyncRelayCommand> CreateNewDeviceCommand = winrt::AsyncRelayCommandFactory::Make([this](winrt::IInspectable const& parameter) -> winrt::IAsyncAction {
            co_await CreateNewDeviceInvoked.invoke();
        });

        eil::fast_event<absl::AnyInvocable<winrt::IAsyncAction()>> CreateNewDeviceInvoked;
    };
}

namespace winrt::SystemExplorer::ViewModels::Settings::factory_implementation
{
    struct RemoteAccessViewModel : RemoteAccessViewModelT<RemoteAccessViewModel, implementation::RemoteAccessViewModel>
    {
    };
}
