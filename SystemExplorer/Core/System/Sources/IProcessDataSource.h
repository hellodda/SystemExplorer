#pragma once
#include <Core/System/N2/process.h>

namespace winrt::SystemExplorer::Core::System::Sources
{
	__interface IProcessDataSource
	{
		[[nodiscard]] HRESULT Enum(
			OUT std::vector<SYSX_PROCESS_ITEM>& source
		) PURE;

		[[nodiscard]] HRESULT Enum(
			OUT std::vector<SYSX_PROCESS_ITEM>& source,
			OUT uint64_t& outSystemTime
		) PURE;

		[[nodiscard]] HRESULT Fill(
			_In_ PSYSX_PROCESS_ITEM item
		) PURE;
	};
}