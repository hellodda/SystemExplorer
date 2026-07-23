#pragma once

#include "Views/Pages/Settings/EmsPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct EmsPage : EmsPageT<EmsPage>
    {
        EmsPage()
        {
         
        }

    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct EmsPage : EmsPageT<EmsPage, implementation::EmsPage>
    {
    };
}
