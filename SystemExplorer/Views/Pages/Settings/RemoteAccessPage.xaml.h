#pragma once

#include "Views/Pages/Settings/RemoteAccessPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct RemoteAccessPage : RemoteAccessPageT<RemoteAccessPage>
    {
        RemoteAccessPage();

        wil::single_threaded_property<winrt::SystemExplorer::ViewModels::Settings::RemoteAccessViewModel> ViewModel;

        winrt::IAsyncAction OnAddDeviceButtonClicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
        {
            co_await showDialogAsync();
        }

        winrt::IAsyncAction OnComputerItemDoubleTapped(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Microsoft::UI::Xaml::Input::DoubleTappedRoutedEventArgs const& e
        )
        {

        }

    private:
        // test
        [[nodiscard]] winrt::IAsyncAction showDialogAsync();
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct RemoteAccessPage : RemoteAccessPageT<RemoteAccessPage, implementation::RemoteAccessPage>
    {
    };
}
