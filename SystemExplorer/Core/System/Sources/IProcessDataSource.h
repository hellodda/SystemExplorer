#pragma once
#include <Core/System/N2/process.h>

namespace winrt::SystemExplorer::Core::System::Sources
{
	__interface IProcessDataSource
	{
		[[nodiscard]] virtual HRESULT Enum(
			std::vector<SYSX_PROCESS_ITEM>& source
		) PURE;

		[[nodiscard]] virtual HRESULT Fill(
			_In_ PSYSX_PROCESS_ITEM item
		) PURE;
	};
}