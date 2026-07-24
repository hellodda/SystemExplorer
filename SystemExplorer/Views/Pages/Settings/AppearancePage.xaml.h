#pragma once

#include "Views/Pages/Settings/AppearancePage.g.h"

#include <ViewModels/Settings/AppearanceViewModel.h>

#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct AppearancePage : AppearancePageT<AppearancePage>
    {
        AppearancePage() = default;

        wil::single_threaded_property<ViewModels::Settings::AppearanceViewModel> ViewModel;
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Settings, AppearancePage);