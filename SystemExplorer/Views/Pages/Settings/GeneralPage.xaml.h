#pragma once

#include "Views/Pages/Settings/GeneralPage.g.h"

#include <ViewModels/Settings/GeneralViewModel.h>

#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct GeneralPage : GeneralPageT<GeneralPage>
    {
        GeneralPage() = default;

        wil::single_threaded_property<ViewModels::Settings::GeneralViewModel> ViewModel;
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Settings, GeneralPage);