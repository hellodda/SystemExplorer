#pragma once

namespace winrt::SystemExplorer::Helpers
{
	struct Win32Helper
	{
		[[nodiscard]] static std::wstring GetErrorMessage(DWORD code);
		[[nodiscard]] static std::wstring GetLocalizedResource(UINT id);
		[[nodiscard]] static std::wstring GetCurrentProcessPath();
	};
}