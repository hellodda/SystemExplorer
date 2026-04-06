#pragma once

#include "Views/Pages/Settings/GeneralPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct GeneralPage : GeneralPageT<GeneralPage>
    {
        GeneralPage()
        {
        
        }
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct GeneralPage : GeneralPageT<GeneralPage, implementation::GeneralPage>
    {
    };
}
