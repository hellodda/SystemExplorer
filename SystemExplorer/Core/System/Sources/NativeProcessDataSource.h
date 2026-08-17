#pragma once
#include "IProcessDataSource.h"

namespace winrt::SystemExplorer::Core::System::Sources
{
	struct NativeProcessDataSource : IProcessDataSource
	{
		[[nodiscard]] HRESULT Enum(
			std::vector<SYSX_PROCESS_ITEM>& source
		) override;

		[[nodiscard]] HRESULT Fill(
			_In_ PSYSX_PROCESS_ITEM item
		) override;

	private:

		void fillProcessItem(
			_Inout_ PSYSX_PROCESS_ITEM item,
			_In_ PSYSTEM_PROCESS_INFORMATION information
		);

	private:
		std::unique_ptr<std::byte[]> buffer_;
		ULONG bufferSize_{ 6400 };
	};	
}

