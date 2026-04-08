#pragma once
#include "Views/Pages/Settings/AppearancePage.g.h"
#include <Core/Settings/UserSettings.h>
#include <winrt/Windows.UI.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    using namespace winrt::Microsoft::UI::Xaml::Media;

    struct AppearancePage : AppearancePageT<AppearancePage>
    {
        AppearancePage()
        {
            
        }
        wil::single_threaded_property<ViewModels::AppearanceViewModel> ViewModel;
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct AppearancePage : AppearancePageT<AppearancePage, implementation::AppearancePage>
    {
    };
}
