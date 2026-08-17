#include "pch.h"
#include "WTSProcessController.h"

namespace winrt::SystemExplorer::Core::System::Controllers
{
	HRESULT WTSProcessController::Server(
		_In_ HANDLE server
	)
	{
		if (server)
		{
			server_ = server;
			return S_OK;
		}
		return E_POINTER;
	}
}