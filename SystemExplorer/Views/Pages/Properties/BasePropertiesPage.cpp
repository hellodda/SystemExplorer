#include "pch.h"
#include "BasePropertiesPage.h"
#if __has_include("Views/Pages/Properties/BasePropertiesPage.g.cpp")
#include "Views/Pages/Properties/BasePropertiesPage.g.cpp"
#endif
#include <Core/Data/Parameters/PropertiesPageNavigationParameter.h>
#include <Core/Data/Items/ProcessItem.h>

using namespace winrt::SystemExplorer::Core::Data::Parameters;
using namespace winrt::SystemExplorer::Core::Data::Items;

namespace winrt::SystemExplorer::Views::Pages::Properties::implementation
{
	void BasePropertiesPage::OnNavigatedTo(NavigationEventArgs const& args)
	{
		auto param = args.Parameter().try_as<PropertiesPageNavigationParameter>();
		
		if (auto process = param.Parameter().try_as<ProcessItem>())
		{

		}

	}
	void BasePropertiesPage::OnNavigatedFrom(NavigationEventArgs const& args)
	{

	}
}
