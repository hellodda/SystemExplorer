#pragma once

namespace winrt::SystemExplorer::Helpers
{
	struct Win32Helper
	{
		static std::wstring GetErrorMessage(DWORD code);
	};
}