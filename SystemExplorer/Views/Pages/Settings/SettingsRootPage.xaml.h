#pragma once

#include "Views/Pages/Settings/SettingsRootPage.g.h"

#include <factory.h>

namespace winrt
{
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Microsoft::UI::Xaml;

    using namespace winrt::Windows::UI::Xaml::Interop;
    using namespace winrt::Windows::Foundation;
}

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct SettingsRootPage : SettingsRootPageT<SettingsRootPage>
    {
        SettingsRootPage() = default;

        [[nodiscard]] winrt::IAsyncAction SettingsFrameLoaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& args);
        [[nodiscard]] winrt::IAsyncAction SettingsNavViewSelectionChanged(winrt::NavigationView const& sender, winrt::NavigationViewSelectionChangedEventArgs const& args);
    };
}

FACTORY(winrt::SystemExplorer::Views::Pages::Settings, SettingsRootPage);