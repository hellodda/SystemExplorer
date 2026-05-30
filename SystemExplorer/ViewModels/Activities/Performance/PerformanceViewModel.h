#pragma once

#include "ViewModels/Activities/Performance/PerformanceViewModel.g.h"
#include <Core/Data/Items/PerformanceNavigationViewItem.h>
#include <factory.h>

namespace winrt::SystemExplorer::ViewModels::Activities::Performance::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::Foundation::Collections;

    using namespace winrt::SystemExplorer::Core::Data::Items;

    struct PerformanceViewModel : PerformanceViewModelT<PerformanceViewModel>
    {
        PerformanceViewModel();
        
        wil::single_threaded_property<IObservableVector<PerformanceNavigationViewItem>> NavigationViewItems = single_threaded_observable_vector<PerformanceNavigationViewItem>();
    };
}
FACTORY(winrt::SystemExplorer::ViewModels::Activities::Performance, PerformanceViewModel);
