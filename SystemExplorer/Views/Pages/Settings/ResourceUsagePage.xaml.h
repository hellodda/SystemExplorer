#pragma once

#include "Views/Pages/Settings/ResourceUsagePage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct ResourceUsagePage : ResourceUsagePageT<ResourceUsagePage>
    {
        ResourceUsagePage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct ResourceUsagePage : ResourceUsagePageT<ResourceUsagePage, implementation::ResourceUsagePage>
    {
    };
}
