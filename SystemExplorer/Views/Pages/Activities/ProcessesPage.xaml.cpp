#include "pch.h"
#include "ProcessesPage.xaml.h"
#if __has_include("Views/Pages/Activities/ProcessesPage.g.cpp")
#include "Views/Pages/Activities/ProcessesPage.g.cpp"
#endif
#include <Models/ProcessInformation.h>
#include <ViewModels/Activities/ProcessesViewModel.h>

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;

using namespace winrt::XamlToolkit::Labs::WinUI;


namespace winrt::SystemExplorer::Views::Pages::Activities::implementation
{
    IAsyncAction ProcessesPage::ProcessMenuFlyoutOpened(IInspectable const& sender, IInspectable const&)
    {
        if (auto flyout = sender.try_as<MenuFlyout>())
        {
            if (auto targetElement = flyout.Target().try_as<FrameworkElement>())
            {
                auto dataContext = targetElement.DataContext();

                if (dataContext)
                {
                    auto selected = dataContext.try_as<SystemExplorer::Models::ProcessInformation>();
                    uint32_t index{ 0 };

                    ViewModel.SelectedProcess(selected);
                    ViewModel.Processes().IndexOf(selected, index);
                    ProcessesList().SelectedIndex(index);
                }
            }
        }
        co_return;
    }
}
