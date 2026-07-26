#pragma once

#include "Views/Pages/Activities/ProcessesPage.g.h"

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Dispatching;
}

namespace winrt::SystemExplorer::Views::Pages::Activities::implementation
{
    struct ProcessesPage : ProcessesPageT<ProcessesPage>
    {
        ProcessesPage()
        {
            InitializeComponent();
        }

        [[nodiscard]] winrt::IAsyncAction ProcessMenuFlyoutOpened(winrt::IInspectable const& sender, winrt::IInspectable const&);

		wil::single_threaded_property<ViewModels::Activities::ProcessesViewModel> ViewModel;

    private:
        winrt::DispatcherQueue dispatcher = winrt::DispatcherQueue::GetForCurrentThread();
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Activities, ProcessesPage);
