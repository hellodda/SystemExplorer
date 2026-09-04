#pragma once

#include "Views/Pages/Properties/ProcessImagePropertiesPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Properties::implementation
{
    struct ProcessImagePropertiesPage : ProcessImagePropertiesPageT<ProcessImagePropertiesPage>
    {
        ProcessImagePropertiesPage()
        {

        }
    };
}

namespace winrt::SystemExplorer::Views::Pages::Properties::factory_implementation
{
    struct ProcessImagePropertiesPage : ProcessImagePropertiesPageT<ProcessImagePropertiesPage, implementation::ProcessImagePropertiesPage>
    {
    };
}
