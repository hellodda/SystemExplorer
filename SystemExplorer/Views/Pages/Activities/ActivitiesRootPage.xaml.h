#pragma once

#include "Views/Pages/Activities/ActivitiesRootPage.g.h"

#include <factory.h>

namespace winrt
{
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Media::Animation;
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::UI::Xaml::Interop;
}

namespace winrt::SystemExplorer::Views::Pages::Activities::implementation
{
    struct ActivitiesRootPage : ActivitiesRootPageT<ActivitiesRootPage>
    {
        ActivitiesRootPage() = default;

        [[nodiscard]] winrt::IAsyncAction ActivitiesFrameLoaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& args);
        [[nodiscard]] winrt::IAsyncAction ActivitiesNavViewSelectionChanged(winrt::NavigationView const& sender, winrt::NavigationViewSelectionChangedEventArgs const& args);
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Activities, ActivitiesRootPage);