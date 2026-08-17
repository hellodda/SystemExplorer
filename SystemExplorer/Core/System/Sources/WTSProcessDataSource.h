#pragma once
#include "IProcessDataSource.h"


namespace winrt::SystemExplorer::Core::System::Sources
{
	struct WTSProcessDataSource : IProcessDataSource
	{
		[[nodiscard]] HRESULT Enum(
			std::vector<SYSX_PROCESS_ITEM>& source
		) override;

		[[nodiscard]] HRESULT Fill(
			_In_ PSYSX_PROCESS_ITEM item
		) override
		{
			return E_NOTIMPL;
		}

		[[nodiscard]] HRESULT Server(
			_In_ HANDLE server
		);

	private:
		HANDLE server_{ ((HANDLE)0) };

		PWTS_PROCESS_INFO_EXW lastBuffer_{ nullptr };
		DWORD lastCount_{ 0 };
	};
}