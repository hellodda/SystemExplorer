#pragma once

#include "Views/Pages/Activities/ServicesPage.g.h"

#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Activities::implementation
{
    struct ServicesPage : ServicesPageT<ServicesPage>
    {
        ServicesPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Activities, ServicesPage);