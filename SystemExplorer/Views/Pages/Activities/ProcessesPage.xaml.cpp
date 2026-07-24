#include "pch.h"
#include "winrt_module_imports.h"
#include "ProcessesPage.xaml.h"
#if __has_include("Views/Pages/Activities/ProcessesPage.g.cpp")
#include "Views/Pages/Activities/ProcessesPage.g.cpp"
#endif
#include <Core/Data/Items/ProcessItem.h>
#include <ViewModels/Activities/ProcessesViewModel.h>
#include <Helpers/ProcessPropertiesHelper.h>


namespace winrt::SystemExplorer::Views::Pages::Activities::implementation
{
    winrt::IAsyncAction ProcessesPage::ProcessMenuFlyoutOpened(winrt::IInspectable const& sender, winrt::IInspectable const&)
    {
        if (auto flyout = sender.try_as<winrt::MenuFlyout>())
        {
            if (auto targetElement = flyout.Target().try_as<winrt::FrameworkElement>())
            {
                auto dataContext = targetElement.DataContext();

                if (dataContext)
                {
                    auto selected = dataContext.try_as<SystemExplorer::Core::Data::Items::ProcessItem>();
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
