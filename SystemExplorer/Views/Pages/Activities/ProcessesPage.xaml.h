#pragma once

#include "Views/Pages/Activities/ProcessesPage.g.h"
#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Activities::implementation
{
    using namespace winrt::Windows::Foundation;

    struct ProcessesPage : ProcessesPageT<ProcessesPage>
    {
        ProcessesPage()
        {
            InitializeComponent();

		
        }
        IAsyncAction ProcessMenuFlyoutOpened(IInspectable const& sender, IInspectable const&);

		wil::single_threaded_property<ViewModels::Activities::ProcessesViewModel> ViewModel;

        void TEST_CLICK(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Activities, ProcessesPage);
