#pragma once
#include "Views/Pages/RootPage.g.h"
#include <ViewModels/RootViewModel.h>
#include <factory.h>

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Windows::UI::Xaml::Interop;
}

namespace winrt::SystemExplorer::Views::Pages::implementation
{
    struct RootPage : RootPageT<RootPage>
    {
        RootPage();

        wil::single_threaded_property<ViewModels::RootViewModel> ViewModel;

        [[nodiscard]] winrt::IAsyncAction NavViewSelectionChanged(winrt::NavigationView const& sender, winrt::NavigationViewSelectionChangedEventArgs const& args);
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages, RootPage);