#pragma once
#include "Views/Pages/Settings/SettingsRootPage.g.h"

#include <winrt/Microsoft.UI.Xaml.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Windows::Foundation;

    struct SettingsRootPage : SettingsRootPageT<SettingsRootPage>
    {
        SettingsRootPage()
        {

        }
        IAsyncAction SettingsFrameLoaded(IInspectable const& sender, RoutedEventArgs const& args);
        IAsyncAction SettingsNavViewSelectionChanged(NavigationView const& sender, NavigationViewSelectionChangedEventArgs const& args);
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct SettingsRootPage : SettingsRootPageT<SettingsRootPage, implementation::SettingsRootPage>
    {
    };
}
