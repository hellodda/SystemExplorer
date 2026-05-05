#pragma once

namespace winrt::SystemExplorer::Helpers
{
	struct Win32Helper
	{
		[[nodiscard]] static std::wstring GetErrorMessage(DWORD code);
		[[nodiscard]] static std::wstring GetLocalizedResource(UINT id);
		[[nodiscard]] static std::wstring GetCurrentProcessPath();
		[[nodiscard]] static std::uint64_t GetCurrentSystemTime();
		[[nodiscard]] static std::uint64_t FileTimeToInteger(FILETIME const& fileTime);
		[[nodiscard]] static std::wstring GetFileVersionString(HANDLE handle, std::wstring_view key);

		struct ProcessHelper
		{
			[[nodiscard]] static std::wstring GetProcessDescription(uint32_t id);
			[[nodiscard]] static std::wstring GetProcessDescription(HANDLE process);
			[[nodiscard]] static std::wstring GetProcessAMUID(uint32_t id);
			[[nodiscard]] static std::wstring GetProcessAMUID(HANDLE process);
			[[nodiscard]] static wil::unique_hicon GetProcessIcon(uint32_t id);
			[[nodiscard]] static wil::unique_hicon GetProcessIcon(HANDLE process);
			[[nodiscard]] static std::wstring GetProcessPath(HANDLE process);
			[[nodiscard]] static bool IsProcessEfficiencyModeEnabled(uint32_t id);
			[[nodiscard]] static bool IsProcessEfficiencyModeEnabled(HANDLE process);
		};
	};
}
