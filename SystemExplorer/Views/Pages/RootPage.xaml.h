#pragma once

#include "Views/Pages/RootPage.g.h"
#include <ViewModels/RootViewModel.h>
#include <macro.h>

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

        wil::single_threaded_property<ViewModels::RootViewModel> ViewModel;

        IAsyncAction NavViewSelectionChanged(NavigationView const& sender, NavigationViewSelectionChangedEventArgs const& args);
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages, RootPage);