#pragma once

namespace winrt::SystemExplorer::Core::System::Controllers
{
	struct ProcessControllerAccess
	{
		bool CanLaunch;
		bool CanTerminate;
		bool CanDump;
		bool CanSuspend;
		bool CanRestart;
		bool CanOpenProcessLocation;
	};

	__interface IProcessController
	{
		[[nodiscard]] ProcessControllerAccess Access() PURE;

		[[nodiscard]] HRESULT Terminate(
			HANDLE process
		) PURE;
	};
}