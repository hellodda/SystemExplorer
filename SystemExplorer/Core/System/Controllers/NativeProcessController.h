#pragma once
#include "IProcessController.h"

namespace winrt::SystemExplorer::Core::System::Controllers
{
	struct NativeProcessController : IProcessController
	{
		[[nodiscard]] ProcessControllerAccess Access() override
		{
			ProcessControllerAccess access;
			access.CanDump = true;
			access.CanLaunch = true;
			access.CanOpenProcessLocation = true;
			access.CanSuspend = true;
			access.CanTerminate = true;

			return access;
		}
	};
}