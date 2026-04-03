#pragma once

#include "Views/Pages/RootPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::implementation
{
	using namespace winrt::Windows::Foundation;
	using namespace winrt::Microsoft::UI::Xaml;
	using namespace winrt::Microsoft::UI::Xaml::Controls;

    struct RootPage : RootPageT<RootPage>
    {
        RootPage()
        {

        }
        IAsyncAction NavViewSelectionChanged(NavigationView const& sender, NavigationViewSelectionChangedEventArgs const& args);
    };
}

namespace winrt::SystemExplorer::Views::Pages::factory_implementation
{
    struct RootPage : RootPageT<RootPage, implementation::RootPage>
    {
    };
}
