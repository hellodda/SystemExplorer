#pragma once

#include "Views/Pages/Settings/AboutPage.g.h"
#include <wil/cppwinrt_authoring.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
	using namespace winrt::SystemExplorer::ViewModels;

    struct AboutPage : AboutPageT<AboutPage>
    {
        AboutPage()
        {
    
        }
        wil::single_threaded_property<ViewModels::AboutViewModel> ViewModel{};
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct AboutPage : AboutPageT<AboutPage, implementation::AboutPage>
    {
    };
}
