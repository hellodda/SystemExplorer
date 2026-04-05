#pragma once
#include "Views/Pages/Settings/AppearancePage.g.h"
#include <Services/UserSettingsService.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
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
