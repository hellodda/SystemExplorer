#pragma once
#include <ems.h>

namespace winrt::SystemExplorer::Core::Ems
{
	struct EmsHost
	{
	public:

		[[nodiscard]] NTSTATUS Start();
		void Stop();

	private:
		NTSTATUS CreateAlpcPort();
		fire_and_forget PortListenerLoopAsync();

		void HandleModuleConnection(HANDLE currentPort, EMS_API_MESSAGE const& message);
		fire_and_forget HandleMessageAsync(HANDLE currentPort, EMS_API_MESSAGE message);
		void HandleModuleDisconnect(EMS_API_MESSAGE const& message);


	};
}