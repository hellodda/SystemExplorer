#include "pch.h"
#include "PerformanceRootPage.xaml.h"
#if __has_include("Views/Pages/Activities/Performance/PerformanceRootPage.g.cpp")
#include "Views/Pages/Activities/Performance/PerformanceRootPage.g.cpp"
#endif
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <Core/Data/Items/PerformanceNavigationViewItem.h>

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Interop;

namespace winrt::SystemExplorer::Views::Pages::Activities::Performance::implementation
{
	IAsyncAction PerformanceRootPage::PerformanceNavViewSelectionChanged(NavigationView const& sender, NavigationViewSelectionChangedEventArgs const& args)
	{
        if (auto selectedItem = args.SelectedItem())
        {
            if (auto item = selectedItem.as<SystemExplorer::Core::Data::Items::PerformanceNavigationViewItem>())
            {
                TypeName typeName{ item.PageName(), TypeKind::Custom};
                ActivitiesFrame().Navigate(typeName, nullptr);
            }
        }
        co_return;
	}
}
