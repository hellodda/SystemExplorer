#pragma once

namespace eil // explorer implementation library
{
	[[nodiscard]] inline std::wstring to_wstring(
		_In_ UNICODE_STRING const& string
	)
	{
		return std::wstring(string.Buffer, string.Length / sizeof(WCHAR));
	}

	[[nodiscard]] inline std::wstring_view to_wstring_view(
		_In_ UNICODE_STRING const& string
	)
	{
		return std::wstring_view(string.Buffer, string.Length / sizeof(WCHAR));
	}

	[[nodiscard]] inline uint64_t get_system_memory()
	{
		SYSTEM_BASIC_INFORMATION basic{};
		SYSTEM_PERFORMANCE_INFORMATION performance{};

#ifdef SYSX_Memory_H
		if (SYSX_IMPL_GetSystemBasicInformation(&basic) < 0)
			return -1;
		if (SYSX_IMPL_GetSystemPerformanceInformation(&performance) < 0)
			return -1;
#endif
		return performance.AvailablePages * basic.PageSize;
	}

	[[nodiscard]] inline uint64_t get_current_system_time()
	{
		FILETIME idleTime{}, kernelTime{}, userTime{};

		if (!GetSystemTimes(&idleTime, &kernelTime, &userTime))
			return 0;
		return std::bit_cast<uint64_t>(kernelTime) + std::bit_cast<uint64_t>(userTime);
	}
}