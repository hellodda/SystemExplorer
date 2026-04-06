#pragma once

#include "Views/Pages/Settings/AdvancedPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct AdvancedPage : AdvancedPageT<AdvancedPage>
    {
        AdvancedPage()
        {
        
        }
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct AdvancedPage : AdvancedPageT<AdvancedPage, implementation::AdvancedPage>
    {
    };
}
