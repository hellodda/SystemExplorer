#pragma once

#include "Views/Pages/Activities/Performance/PerformanceRootPage.g.h"

#include <factory.h>

namespace winrt
{
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Windows::UI::Xaml::Interop;
    using namespace winrt::Windows::Foundation;
}

namespace winrt::SystemExplorer::Views::Pages::Activities::Performance::implementation
{
    struct PerformanceRootPage : PerformanceRootPageT<PerformanceRootPage>
    {
        PerformanceRootPage() = default;
        
        [[nodiscard]] winrt::IAsyncAction PerformanceNavViewSelectionChanged(winrt::NavigationView const& sender, winrt::NavigationViewSelectionChangedEventArgs const& args);

        wil::single_threaded_property<ViewModels::Activities::Performance::PerformanceViewModel> ViewModel;
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Activities::Performance, PerformanceRootPage);