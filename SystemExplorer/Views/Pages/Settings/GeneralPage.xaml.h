#pragma once
#include "Views/Pages/Settings/GeneralPage.g.h"

#include <wil/cppwinrt_authoring.h>
#include <ViewModels/Settings/GeneralViewModel.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct GeneralPage : GeneralPageT<GeneralPage>
    {
        GeneralPage()
        {
            InitializeComponent();
        }
        wil::single_threaded_property<ViewModels::Settings::GeneralViewModel> ViewModel;
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Settings, GeneralPage);