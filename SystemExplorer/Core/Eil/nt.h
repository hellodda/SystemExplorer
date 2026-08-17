#pragma once
#include <Core/System/N2/memory.h>

#include <phnt_ntdef.h>
#include <string>

namespace eil::nt
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

		if (SYSX_IMPL_GetSystemBasicInformation(&basic) < 0)
			return -1;
		if (SYSX_IMPL_GetSystemPerformanceInformation(&performance) < 0)
			return -1;

		return performance.AvailablePages * basic.PageSize;
	}
}