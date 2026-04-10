#pragma once

#include "Views/Pages/Settings/AboutPage.g.h"
#include <ViewModels/Settings/AboutViewModel.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
	using namespace winrt::SystemExplorer::ViewModels;

    struct AboutPage : AboutPageT<AboutPage>
    {
        AboutPage()
        {
    
        }
        wil::single_threaded_property<ViewModels::Settings::AboutViewModel> ViewModel{};
    };
}

FACTORY(winrt::SystemExplorer::Views::Pages::Settings, AboutPage);