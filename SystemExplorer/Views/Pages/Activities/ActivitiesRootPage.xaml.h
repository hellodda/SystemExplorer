#pragma once

#include "Views/Pages/Activities/ActivitiesRootPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Activities::implementation
{
    struct ActivitiesRootPage : ActivitiesRootPageT<ActivitiesRootPage>
    {
        ActivitiesRootPage()
        {
      
        }
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Activities, ActivitiesRootPage);