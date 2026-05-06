#pragma once
#include <Core/Data/Items/AppThemeResourceItem.h>

namespace winrt::SystemExplorer::Core::Data::Factories
{
	using namespace winrt::Windows::Foundation::Collections;

	struct AppThemeResourceFactory
	{
		[[nodiscard]] static IObservableVector<SystemExplorer::Core::Data::Items::AppThemeResourceItem> AppThemeResources();
	};
}

