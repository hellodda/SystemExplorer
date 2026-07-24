#pragma once

#include "Views/Pages/Settings/AboutPage.g.h"

#include <ViewModels/Settings/AboutViewModel.h>

#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct AboutPage : AboutPageT<AboutPage>
    {
        AboutPage() = default;

        wil::single_threaded_property<ViewModels::Settings::AboutViewModel> ViewModel{};
    };
}

FACTORY(winrt::SystemExplorer::Views::Pages::Settings, AboutPage);