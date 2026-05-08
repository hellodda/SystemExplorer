#pragma once
#include "BasePropertiesPage.h"

#include "Views/Pages/Properties/PropertiesGeneralPage.g.h"
#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Properties::implementation
{
    struct PropertiesGeneralPage : PropertiesGeneralPageT<PropertiesGeneralPage>
    {
        PropertiesGeneralPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Properties, PropertiesGeneralPage);