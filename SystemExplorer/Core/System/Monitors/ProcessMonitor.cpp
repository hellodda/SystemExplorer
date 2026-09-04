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
        if (FAILED(dataSource_->Enum(rawSnapshotBuffer_, currentSystemTime)))
        {
            return;
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

                cached.CpuKernelDelta.Update(raw.KernelTime.QuadPart);
                cached.CpuUserDelta.Update(raw.UserTime.QuadPart);

                cached.IoReadDelta.Update(raw.IoReadDelta.Value);
                cached.IoWriteDelta.Update(raw.IoWriteDelta.Value);
                cached.IoOtherDelta.Update(raw.IoOtherDelta.Value);

                cached.IoReadCountDelta.Update(raw.IoReadCountDelta.Value);
                cached.IoWriteCountDelta.Update(raw.IoWriteCountDelta.Value);
                cached.IoOtherCountDelta.Update(raw.IoOtherCountDelta.Value);

                cached.PageFaultsDelta.Update(raw.PageFaultsDelta.Value);

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