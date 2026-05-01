#pragma once

#include "Views/Pages/Activities/ActivitiesRootPage.g.h"
#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Activities::implementation
{
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Windows::Foundation;

    struct ActivitiesRootPage : ActivitiesRootPageT<ActivitiesRootPage>
    {
        ActivitiesRootPage()
        {
      
        }
        IAsyncAction ActivitiesFrameLoaded(IInspectable const& sender, RoutedEventArgs const& args);
        IAsyncAction ActivitiesNavViewSelectionChanged(NavigationView const& sender, NavigationViewSelectionChangedEventArgs const& args);
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Activities, ActivitiesRootPage);