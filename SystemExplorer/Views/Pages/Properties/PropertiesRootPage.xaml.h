#pragma once

#include "Views/Pages/Properties/PropertiesRootPage.g.h"

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Windows::UI::Xaml::Interop;
}

namespace winrt::SystemExplorer::Views::Pages::Properties::implementation
{
    struct PropertiesRootPage : PropertiesRootPageT<PropertiesRootPage>
    {
        PropertiesRootPage()
        {
        }
        
        wil::single_threaded_property<winrt::IObservableVector<Core::Data::Items::ObjectPairItem>> Properties =
            winrt::single_threaded_observable_vector<Core::Data::Items::ObjectPairItem>();

        [[nodiscard]] winrt::IAsyncAction NavViewSelectionChanged(winrt::NavigationView const& sender, winrt::NavigationViewSelectionChangedEventArgs const& args);
    };
}

namespace winrt::SystemExplorer::Views::Pages::Properties::factory_implementation
{
    struct PropertiesRootPage : PropertiesRootPageT<PropertiesRootPage, implementation::PropertiesRootPage>
    {
    };
}
