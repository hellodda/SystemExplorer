#include "pch.h"
#include "winrt_module_imports.h"
#include "PerformanceRootPage.xaml.h"
#if __has_include("Views/Pages/Activities/Performance/PerformanceRootPage.g.cpp")
#include "Views/Pages/Activities/Performance/PerformanceRootPage.g.cpp"
#endif

#include <Core/Data/Items/PerformanceNavigationViewItem.h>

namespace winrt::SystemExplorer::Views::Pages::Activities::Performance::implementation
{
	IAsyncAction PerformanceRootPage::PerformanceNavViewSelectionChanged(winrt::NavigationView const& sender, winrt::NavigationViewSelectionChangedEventArgs const& args)
	{
        if (auto selectedItem = args.SelectedItem())
        {
            if (auto item = selectedItem.as<SystemExplorer::Core::Data::Items::PerformanceNavigationViewItem>())
            {
                winrt::TypeName typeName{ item.PageName(), winrt::TypeKind::Custom};
                ActivitiesFrame().Navigate(typeName, nullptr);
            }
        }
        co_return;
	}
}
