#pragma once
#include "Views/Pages/Settings/GeneralPage.g.h"

#include <wil/cppwinrt_authoring.h>
#include <ViewModels/Settings/GeneralViewModel.h>
#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct GeneralPage : GeneralPageT<GeneralPage>
    {
        GeneralPage()
        {
            InitializeComponent();
        }
        winrt::Windows::Foundation::IAsyncAction TEST(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
        winrt::Windows::Foundation::IAsyncAction TEST1(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);

        wil::single_threaded_property<ViewModels::Settings::GeneralViewModel> ViewModel;
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Settings, GeneralPage);