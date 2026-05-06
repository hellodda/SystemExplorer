#pragma once
#include "BasePropertiesPage.h"
#include "Views/Pages/Properties/PropertiesRootPage.g.h"

#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Properties::implementation
{
    struct PropertiesRootPage : PropertiesRootPageT<PropertiesRootPage>
    {
        PropertiesRootPage()
        {
           
        }
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Properties, PropertiesRootPage);
