#pragma once

#include "Views/Pages/Settings/ResourceUsagePage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct ResourceUsagePage : ResourceUsagePageT<ResourceUsagePage>
    {
        ResourceUsagePage()
        {

        }

    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct ResourceUsagePage : ResourceUsagePageT<ResourceUsagePage, implementation::ResourceUsagePage>
    {
    };
}
