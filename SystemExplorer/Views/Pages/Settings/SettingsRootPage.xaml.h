#pragma once
#include "Views/Pages/Settings/SettingsRootPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Windows::Foundation;

    struct SettingsRootPage : SettingsRootPageT<SettingsRootPage>
    {
        SettingsRootPage()
        {
          
        }
        IAsyncAction SettingsNavViewSelectionChanged(NavigationView const& sender, NavigationViewSelectionChangedEventArgs const& args);
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct SettingsRootPage : SettingsRootPageT<SettingsRootPage, implementation::SettingsRootPage>
    {
    };
}
