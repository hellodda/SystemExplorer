#pragma once

#include "Views/Pages/Settings/DeveloperToolsPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct DeveloperToolsPage : DeveloperToolsPageT<DeveloperToolsPage>
    {
        DeveloperToolsPage()
        {
          
        }
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct DeveloperToolsPage : DeveloperToolsPageT<DeveloperToolsPage, implementation::DeveloperToolsPage>
    {
    };
}
