#pragma once

#include "Views/Pages/Activities/ServicesPage.g.h"

#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Activities::implementation
{
    struct ServicesPage : ServicesPageT<ServicesPage>
    {
        ServicesPage() = default;
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Activities, ServicesPage);
