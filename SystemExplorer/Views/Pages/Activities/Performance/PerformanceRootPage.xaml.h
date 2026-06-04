#pragma once

#include "Views/Pages/Activities/Performance/PerformanceRootPage.g.h"
#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Activities::Performance::implementation
{
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Microsoft::UI::Xaml;

    using namespace winrt::Windows::Foundation;

    struct PerformanceRootPage : PerformanceRootPageT<PerformanceRootPage>
    {
        PerformanceRootPage()
        {
            InitializeComponent();
        }
        IAsyncAction PerformanceNavViewSelectionChanged(NavigationView const& sender, NavigationViewSelectionChangedEventArgs const& args);

        wil::single_threaded_property<ViewModels::Activities::Performance::PerformanceViewModel> ViewModel;
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Activities::Performance, PerformanceRootPage);