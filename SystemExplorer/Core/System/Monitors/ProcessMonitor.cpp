#include "pch.h"
#include "ProcessMonitor.h"

namespace winrt::SystemExplorer::Core::System::Monitors
{
	HRESULT ProcessMonitor::DataSource(std::unique_ptr<Sources::IProcessDataSource> dataSource) noexcept
	{
		if (!dataSource) return E_POINTER;
		dataSource_ = std::move(dataSource);
		return S_OK;
	}

	void ProcessMonitor::OnTimer() { raiseDataCollected(); }
	void ProcessMonitor::OnStart() { raiseDataCollected(); }
	void ProcessMonitor::OnSuspend() {}
	void ProcessMonitor::OnStop() {}

	void ProcessMonitor::raiseDataCollected()
	{
		if (!dataSource_)
			return;

		LOG_IF_FAILED(dataSource_->Enum(rawBuffer_));

		if (rawBuffer_.empty())
		{
			cache_.clear();
			return;
		}

		absl::flat_hash_set<DWORD> currentIds;
		currentIds.reserve(rawBuffer_.size());

		for (auto& rawProcess : rawBuffer_)
		{
			auto pid = static_cast<DWORD>(reinterpret_cast<ULONG_PTR>(rawProcess.ProcessId));
			currentIds.insert(pid);

			auto it = cache_.find(pid);
			if (it != cache_.end())
			{
				auto& cached = it->second;

				if (cached.CreateTime.QuadPart == rawProcess.CreateTime.QuadPart)
				{
					// ВАЖНО: Обновляем указатель на строку, т.к. старый буфер DataSource мог быть удален
					cached.ProcessName = rawProcess.ProcessName;

					cached.IoReadDelta = rawProcess.IoReadDelta;
					cached.IoWriteDelta = rawProcess.IoWriteDelta;
					cached.IoOtherDelta = rawProcess.IoOtherDelta;
					cached.IoReadCountDelta = rawProcess.IoReadCountDelta;
					cached.IoWriteCountDelta = rawProcess.IoWriteCountDelta;
					cached.CpuKernelDelta = rawProcess.CpuKernelDelta;
					cached.CpuUserDelta = rawProcess.CpuUserDelta;
					cached.NumberOfThreads = rawProcess.NumberOfThreads;
					cached.NumberOfHandles = rawProcess.NumberOfHandles;
					cached.WorkingSetPrivateSize = rawProcess.WorkingSetPrivateSize;
					cached.PeakNumberOfThreads = rawProcess.PeakNumberOfThreads;
					cached.VmCounters = rawProcess.VmCounters;
					cached.IoCounters = rawProcess.IoCounters;
					cached.KernelTime = rawProcess.KernelTime;
					cached.UserTime = rawProcess.UserTime;
				}
				else
				{
					cached = rawProcess;
					LOG_IF_FAILED(dataSource_->Fill(&cached));
				}
			}
			else
			{
				LOG_IF_FAILED(dataSource_->Fill(&rawProcess));
				cache_.emplace(pid, rawProcess);
			}
		}

		viewBuffer_.clear();
		viewBuffer_.reserve(cache_.size());

		for (auto it = cache_.begin(); it != cache_.end(); )
		{
			if (!currentIds.contains(it->first))
			{
				cache_.erase(it++);
			}
			else
			{
				viewBuffer_.push_back(&it->second);
				++it;
			}
		}

		OnDataCollected.invoke(std::span<SYSX_PROCESS_ITEM*>{viewBuffer_.data(), viewBuffer_.size()});
	}
}