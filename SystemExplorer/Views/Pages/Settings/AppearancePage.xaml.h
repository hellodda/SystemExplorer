#pragma once
#include "Views/Pages/Settings/AppearancePage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct AppearancePage : AppearancePageT<AppearancePage>
    {
        AppearancePage()
        {
        }
        
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct AppearancePage : AppearancePageT<AppearancePage, implementation::AppearancePage>
    {
    };
}
