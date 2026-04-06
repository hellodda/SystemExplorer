#pragma once
#include <Models/Items/AppThemeResourceItem.h>

namespace winrt::SystemExplorer::Core::Data::Factories
{
	using namespace winrt::Windows::Foundation::Collections;

	struct AppThemeResourceFactory
	{
		[[nodiscard]] static IObservableVector<SystemExplorer::Models::Items::AppThemeResourceItem> AppThemeResources();
	};
}

