#pragma once

#include "Views/Pages/Settings/RemoteAccessPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct RemoteAccessPage : RemoteAccessPageT<RemoteAccessPage>
    {
        RemoteAccessPage()
        {
          
        }

        winrt::Windows::Foundation::IAsyncAction BtnClicked(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct RemoteAccessPage : RemoteAccessPageT<RemoteAccessPage, implementation::RemoteAccessPage>
    {
    };
}
