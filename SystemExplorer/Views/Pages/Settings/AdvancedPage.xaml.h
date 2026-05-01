#pragma once

#include "Views/Pages/Settings/AdvancedPage.g.h"
#include <ViewModels/Settings/AdvancedViewModel.h>
#include <wil/cppwinrt_authoring.h>
#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct AdvancedPage : AdvancedPageT<AdvancedPage>
    {
        AdvancedPage()
        {
        
        }
        wil::single_threaded_property<ViewModels::Settings::AdvancedViewModel> ViewModel;
    };
}

FACTORY(winrt::SystemExplorer::Views::Pages::Settings, AdvancedPage);