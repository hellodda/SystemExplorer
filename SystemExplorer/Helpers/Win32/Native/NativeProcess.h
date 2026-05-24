#pragma once

namespace winrt::SystemExplorer::Helpers::Win32::Native
{
	struct NativeProcess
	{
		[[nodiscard]] static bool IsEfficiencyModeEnabled(uint64_t process);
		[[nodiscard]] static std::wstring GetProcessImageName(uint64_t process);
		[[nodiscard]] static wil::unique_hicon GetProcessIcon(std::wstring const& fileName);
	};
}
