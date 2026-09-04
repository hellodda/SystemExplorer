#pragma once
#include "IProcessController.h"
#include "../N2/process.h"

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

		[[nodiscard]] HRESULT Terminate(
			HANDLE process
		) override
		{
			return HRESULT_FROM_NT(SeTerminateProcess(process, STATUS_SUCCESS));
		}
	};
}