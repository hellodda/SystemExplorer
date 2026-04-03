#pragma once

#include "Views/Pages/Settings/AboutPage.g.h"
#include <wil/cppwinrt_authoring.h>
#include <ViewModels/Pages/Settings/AboutViewModel.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
	using namespace winrt::SystemExplorer::ViewModels::Pages::Settings;

    struct AboutPage : AboutPageT<AboutPage>
    {
        AboutPage()
        {
    
        }
        wil::single_threaded_property<implementation::AboutViewModel> ViewModel{};
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct AboutPage : AboutPageT<AboutPage, implementation::AboutPage>
    {
    };
}
