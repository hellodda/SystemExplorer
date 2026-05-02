#pragma once

namespace winrt::SystemExplorer::Helpers
{
	struct Win32Helper
	{
		[[nodiscard]] static std::wstring GetErrorMessage(DWORD code);
		[[nodiscard]] static std::wstring GetLocalizedResource(UINT id);
		[[nodiscard]] static std::wstring GetCurrentProcessPath();
		[[nodiscard]] static uint64_t GetCurrentSystemTime();
		[[nodiscard]] static uint64_t FileTimeToInteger(FILETIME const& fileTime);
		[[nodiscard]] static std::wstring GetFileVersionString(HANDLE handle, std::wstring_view key);

		struct ProcessHelper
		{
			[[nodiscard]] static std::wstring GetProcessDescription(uint32_t id);
			[[nodiscard]] static HICON GetProcessIcon(uint32_t id);
		};
	};
}