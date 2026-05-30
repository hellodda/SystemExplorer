#pragma once

#include "Views/Pages/Activities/Performance/PerformanceRootPage.g.h"
#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Activities::Performance::implementation
{
    struct PerformanceRootPage : PerformanceRootPageT<PerformanceRootPage>
    {
        PerformanceRootPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
            InitializeComponent();
        }
        wil::single_threaded_property<ViewModels::Activities::Performance::PerformanceViewModel> ViewModel;
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Activities::Performance, PerformanceRootPage);