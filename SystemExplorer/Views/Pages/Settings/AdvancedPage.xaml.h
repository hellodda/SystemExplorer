#pragma once

#include "Views/Pages/Settings/AdvancedPage.g.h"
#include <wil/cppwinrt_authoring.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct AdvancedPage : AdvancedPageT<AdvancedPage>
    {
        AdvancedPage()
        {
        
        }
        wil::single_threaded_property<ViewModels::AdvancedViewModel> ViewModel;
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct AdvancedPage : AdvancedPageT<AdvancedPage, implementation::AdvancedPage>
    {
    };
}
