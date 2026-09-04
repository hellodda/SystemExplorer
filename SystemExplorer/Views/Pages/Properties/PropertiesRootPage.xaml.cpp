#include "pch.h"
#include "PropertiesRootPage.xaml.h"
#if __has_include("Views/Pages/Properties/PropertiesRootPage.g.cpp")
#include "Views/Pages/Properties/PropertiesRootPage.g.cpp"
#endif
#include <Core/Data/Items/ObjectPairItem.h>

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::SystemExplorer::Views::Pages::Properties::implementation
{
	winrt::IAsyncAction PropertiesRootPage::NavViewSelectionChanged(winrt::NavigationView const& sender, winrt::NavigationViewSelectionChangedEventArgs const& args)
	{
		auto item = args.SelectedItem().try_as<SystemExplorer::Core::Data::Items::ObjectPairItem>();

		if (auto type = item.Second().try_as<winrt::TypeName>())
		{
			NavigationFrame().Navigate(type.value());
		}
		co_return;
	}
}

