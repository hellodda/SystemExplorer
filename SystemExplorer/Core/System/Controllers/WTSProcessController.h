#pragma once
#include "IProcessController.h"

namespace winrt::SystemExplorer::Core::System::Controllers
{
	struct WTSProcessController : IProcessController
	{
		[[nodiscard]] ProcessControllerAccess Access() override
		{
			ProcessControllerAccess access;
			return access;
		}

		[[nodiscard]] HRESULT Server(
			_In_ HANDLE server
		);

	private:
		HANDLE server_{ nullptr };
	};
}