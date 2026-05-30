#pragma once

#include "Views/Pages/Activities/ServicesPage.g.h"

#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Activities::implementation
{
    using namespace winrt::Windows::Foundation;

    struct ServicesPage : ServicesPageT<ServicesPage>
    {
        ServicesPage();
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Activities, ServicesPage);