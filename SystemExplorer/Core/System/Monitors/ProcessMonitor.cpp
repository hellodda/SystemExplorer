#include "pch.h"
#include "ProcessMonitor.h"

#include <core/eil/delta.h>

namespace winrt::SystemExplorer::Core::System::Monitors
{
    HRESULT ProcessMonitor::DataSource(std::unique_ptr<Sources::IProcessDataSource> dataSource) noexcept
    {
        if (!dataSource) return E_POINTER;
        dataSource_ = std::move(dataSource);
        return S_OK;
    }

    void ProcessMonitor::OnStart()
    {
        raiseDataCollected();
    }

    void ProcessMonitor::OnTimer()
    {
        raiseDataCollected();
    }

    void ProcessMonitor::OnSuspend() {}

    void ProcessMonitor::OnStop() {}

    void ProcessMonitor::raiseDataCollected()
    {
        if (!dataSource_) return;

        uint64_t currentSystemTime{ 0 };

        auto hr = dataSource_->Enum(rawSnapshotBuffer_, currentSystemTime);

        if (FAILED(hr))
        {

        }

        uint64_t sysDelta = (lastSystemTime_ > 0) ? (currentSystemTime - lastSystemTime_) : 0;

        currentPids_.clear();
        currentPids_.reserve(rawSnapshotBuffer_.size());

        for (auto& raw : rawSnapshotBuffer_)
        {
            currentPids_.insert(raw.ProcessId);
            auto it = cache_.find(raw.ProcessId);

            if (it != cache_.end() && it->second.CreateTime.QuadPart == raw.CreateTime.QuadPart)
            {
                auto& cached = it->second;

                eil::update_native_delta(&cached.CpuKernelDelta, raw.KernelTime.QuadPart);
				eil::update_native_delta(&cached.CpuUserDelta, raw.UserTime.QuadPart);
				eil::update_native_delta(&cached.IoReadDelta, raw.IoReadDelta.Value);
				eil::update_native_delta(&cached.IoWriteDelta, raw.IoWriteDelta.Value);
				eil::update_native_delta(&cached.IoOtherDelta, raw.IoOtherDelta.Value);
				eil::update_native_delta(&cached.IoReadCountDelta, raw.IoReadCountDelta.Value);
				eil::update_native_delta(&cached.IoWriteCountDelta, raw.IoWriteCountDelta.Value);
				eil::update_native_delta(&cached.IoOtherCountDelta, raw.IoOtherCountDelta.Value);
				eil::update_native_delta(&cached.PageFaultsDelta, raw.PageFaultsDelta.Value);

                if (sysDelta > 0)
                {
                    uint64_t totalProcDelta = cached.CpuKernelDelta.Delta + cached.CpuUserDelta.Delta;
                    float rawCpu = static_cast<float>(totalProcDelta * 100.0 / sysDelta);
                    cached.CpuUsage = std::round(rawCpu * 10.0f) / 10.0f;

                    cached.CpuKernelUsage = static_cast<float>(cached.CpuKernelDelta.Delta * 100.0 / sysDelta);
                    cached.CpuUserUsage = static_cast<float>(cached.CpuUserDelta.Delta * 100.0 / sysDelta);
                }

                cached.ProcessName = raw.ProcessName;
                cached.NumberOfThreads = raw.NumberOfThreads;
                cached.NumberOfHandles = raw.NumberOfHandles;
                cached.WorkingSetPrivateSize = raw.WorkingSetPrivateSize;
                cached.PeakNumberOfThreads = std::max(cached.PeakNumberOfThreads, raw.NumberOfThreads);
                cached.VmCounters = raw.VmCounters;
                cached.IoCounters = raw.IoCounters;
            }
            else
            {
                SYSX_PROCESS_ITEM newItem = std::move(raw);
                dataSource_->Fill(&newItem);
                cache_[newItem.ProcessId] = std::move(newItem);
            }
        }

        absl::erase_if(cache_, [this](const auto& pair)
        {
            return !currentPids_.contains(pair.first);
        });

        lastSystemTime_ = currentSystemTime;

        viewBuffer_.clear();
        viewBuffer_.reserve(cache_.size());

        for (auto& [pid, item] : cache_)
        {
            viewBuffer_.push_back(&item);
        }
        OnDataCollected.invoke(std::span<PSYSX_PROCESS_ITEM>{ viewBuffer_.data(), viewBuffer_.size() });
    }
}