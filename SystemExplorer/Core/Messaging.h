#pragma once
#include <eventbus/event_bus.hpp>

namespace winrt::SystemExplorer::Core::Messaging
{
	inline static dp::event_bus ViewModelsGlobalMessenger;

	struct DataSourceChangedMessage
	{
		winrt::SystemExplorer::Core::Data::Items::RemoteComputerItem DataItem;
	};
}