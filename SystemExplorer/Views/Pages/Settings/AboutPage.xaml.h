#pragma once

#include "Views/Pages/Settings/AboutPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct AboutPage : AboutPageT<AboutPage>
    {
        AboutPage()
        {
    
        }
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct AboutPage : AboutPageT<AboutPage, implementation::AboutPage>
    {
    };
}
