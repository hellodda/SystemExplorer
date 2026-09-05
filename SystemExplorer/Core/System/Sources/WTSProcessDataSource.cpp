#include "pch.h"
#include "WTSProcessDataSource.h"
#include <core/eil/nt.h>

namespace winrt::SystemExplorer::Core::System::Sources
{
	HRESULT WTSProcessDataSource::Enum(std::vector<SYSX_PROCESS_ITEM>& source)
	{
		PWTS_PROCESS_INFO_EXW process{ nullptr };
		DWORD count{ 0 };
		DWORD level{ 1 };

		// Освобождаем буфер от ПРЕДЫДУЩЕГО тика
		if (lastBuffer_)
		{
			WTSFreeMemoryExW(WTSTypeProcessInfoLevel1, lastBuffer_, lastCount_);
			lastBuffer_ = nullptr;
		}

		if (WTSEnumerateProcessesExW(server_, &level, WTS_ANY_SESSION, (LPWSTR*)&process, &count))
		{
			// Сохраняем буфер, чтобы указатели ProcessName оставались валидными!
			lastBuffer_ = process;
			lastCount_ = count;

			source.clear();
			source.reserve(count);

			for (auto i = 0; i < count; ++i)
			{
				SYSX_PROCESS_ITEM item{ 0 };

				item.ProcessId = reinterpret_cast<HANDLE>(static_cast<ULONG_PTR>(process[i].ProcessId));
				item.SessionId = process[i].SessionId;

				RtlInitUnicodeString(&item.ProcessName, process[i].pProcessName);

				item.Sid = process[i].pUserSid;
				item.NumberOfThreads = process[i].NumberOfThreads;
				item.NumberOfHandles = process[i].HandleCount;
				item.KernelTime = process[i].KernelTime;
				item.UserTime = process[i].UserTime;
				item.VmCounters.PagefileUsage = process[i].PagefileUsage;
				item.VmCounters.PeakPagefileUsage = process[i].PeakPagefileUsage;
				item.VmCounters.WorkingSetSize = process[i].WorkingSetSize;
				item.VmCounters.PeakWorkingSetSize = process[i].PeakWorkingSetSize;
				item.WorkingSetPrivateSize = 0;

				source.push_back(item);
			}
		}
		else
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}
		return S_OK;
	}

	HRESULT WTSProcessDataSource::Server(
		_In_ HANDLE server
	)
	{
		if (server)
		{
			server_ = server;
			return S_OK;
		}
		return E_POINTER;
	}
}