#pragma once
#include <eventbus/event_bus.hpp>

namespace winrt::SystemExplorer::Core::Messaging
{
	inline static dp::event_bus ViewModelsGlobalMessenger;

	struct LogAnyInformationMessage
	{
		std::wstring Message;
		bool t1;
		bool t2;
		bool t3;
	};

#ifdef WINRT_SystemExplorer_Core_Data_Items_0_H
	
	struct DataSourceChangedMessage
	{
		winrt::SystemExplorer::Core::Data::Items::RemoteComputerItem DataItem;
	};

#endif // WINRT_SystemExplorer_Core_Data_Items_0_H

}