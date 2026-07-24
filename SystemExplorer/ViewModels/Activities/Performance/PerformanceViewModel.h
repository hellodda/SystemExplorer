#pragma once

#include "ViewModels/Activities/Performance/PerformanceViewModel.g.h"
#include <Core/Data/Items/PerformanceNavigationViewItem.h>
#include <factory.h>

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::Foundation::Collections;

    using namespace winrt::SystemExplorer::Core::Data::Items;
}

namespace winrt::SystemExplorer::ViewModels::Activities::Performance::implementation
{
    struct PerformanceViewModel : PerformanceViewModelT<PerformanceViewModel>
    {
        PerformanceViewModel();
        
        wil::single_threaded_property<winrt::IObservableVector<winrt::PerformanceNavigationViewItem>> NavigationViewItems = winrt::single_threaded_observable_vector<winrt::PerformanceNavigationViewItem>();
    };
}
FACTORY(winrt::SystemExplorer::ViewModels::Activities::Performance, PerformanceViewModel);
