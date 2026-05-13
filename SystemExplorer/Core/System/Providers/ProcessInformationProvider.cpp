#include "pch.h"
#include "ProcessInformationProvider.h"
#include <unordered_set>
#include "../Native/process.h"
#include "../Native/procproc.h"
#include "../Native/util.h"
#include "../Utils.h"

namespace winrt::SystemExplorer::Core::System::Providers
{
    ProcessInformationProvider::ProcessInformationProvider()
    {
        registry_ = thread_.Register([this]() {
            updateProcesses();
        });
        registry_->Enable();
        thread_.Start(std::chrono::milliseconds(500));
    }

    std::vector<ProcessNativeInformation> ProcessInformationProvider::GetAllProcesses()
    {
        auto lock = lock_.lock_shared();
        return activeProcesses_;
    }

    ProviderThread& ProcessInformationProvider::Thread()
    {
        return thread_;
    }

    void ProcessInformationProvider::updateProcesses()
    {
      /*  auto currentSystemTime = Win32Helper::GetCurrentSystemTime();
        auto currentTick = GetTickCount64();

        auto buffer = Utils::nt_safe_wrapper([](auto buffer, auto size, auto returnLength) {
            return NtQuerySystemInformation(SystemProcessInformation, buffer, size, returnLength);
        });

        std::vector<ProcessNativeInformation> newActiveProcesses;
        std::unordered_set<uint32_t> currentTickPids;

        auto* pHead = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(buffer.get());

        for (auto& pInfo : Utils::nt_make_range(pHead))
        {
            uint32_t pid = static_cast<uint32_t>(reinterpret_cast<ULONG_PTR>(pInfo.UniqueProcessId));
            currentTickPids.insert(pid);

            newActiveProcesses.push_back(parseCacheProcess(&pInfo, pid, currentSystemTime, currentTick));
        }
        {
            auto lock = lock_.lock_exclusive();
            activeProcesses_ = std::move(newActiveProcesses);
        }
        cleanupCache(currentTickPids);*/
    }

    ProcessNativeInformation ProcessInformationProvider::parseCacheProcess(
        _In_ PSYSTEM_PROCESS_INFORMATION pInfo,
        IN uint32_t pid,
        IN uint64_t currentSystemTime,
        IN uint64_t currentTick
    )
    {
        ProcessNativeInformation pi{};
       /* ProcessNativeInformation pi{};
        pi.Pid = pid;
        pi.ParentId = static_cast<uint32_t>(reinterpret_cast<ULONG_PTR>(pInfo->InheritedFromUniqueProcessId));
        pi.PrivateBytes = static_cast<uint32_t>(pInfo->PrivatePageCount);
        pi.CpuUsage = 0.0f;
        pi.IoRate = 0;
        pi.Status = Utils::GetProcessStatus(pInfo);

        auto currentIoCount = pInfo->ReadTransferCount.QuadPart + pInfo->WriteTransferCount.QuadPart + pInfo->OtherTransferCount.QuadPart;
        auto currentProcessTime = pInfo->KernelTime.QuadPart + pInfo->UserTime.QuadPart;

        auto it = processCache_.find(pid);

        if (it != processCache_.end() && it->second.CreateTime.QuadPart == pInfo->CreateTime.QuadPart)
        {
            ProcessCacheEntry& cache = it->second;
            cache.IsActive = true;

            auto sysDelta = currentSystemTime - cache.LastSystemTime;
            auto procDelta = currentProcessTime - cache.LastProcessTime;

            if (sysDelta > 0)
            {
                float rawCpu = static_cast<float>(procDelta * 100.0 / sysDelta);
                pi.CpuUsage = std::round(rawCpu * 10.0f) / 10.0f;
            }

            auto tickDelta = currentTick - cache.LastTickCount;
            if (tickDelta > 0)
            {
                auto ioDelta = currentIoCount - cache.LastIoTransferCount;
                pi.IoRate = static_cast<uint32_t>((ioDelta * 1000ULL) / tickDelta);
            }

            cache.LastSystemTime = currentSystemTime;
            cache.LastProcessTime = currentProcessTime;
            cache.LastIoTransferCount = currentIoCount;
            cache.LastTickCount = currentTick;

            pi.Name = cache.Name.c_str();
            pi.Description = cache.Description.c_str();
            pi.IsEfficiencyModeEnabled = cache.IsEfficiencyModeEnabled;
            pi.Icon = cache.Icon.get();
        }
        else
        {
            wil::unique_handle handle{ OpenProcess(
                PROCESS_QUERY_LIMITED_INFORMATION,
                FALSE,
                pid
            ) };

            ProcessCacheEntry newEntry{};
            newEntry.CreateTime = pInfo->CreateTime;
            newEntry.LastSystemTime = currentSystemTime;
            newEntry.LastProcessTime = currentProcessTime;
            newEntry.LastIoTransferCount = currentIoCount;
            newEntry.LastTickCount = currentTick;
            newEntry.IsActive = true;
            newEntry.IsEfficiencyModeEnabled = NativeProcess::IsProcessEfficiencyModeEnabled(handle.get());
            newEntry.LastStatusCheckTick = currentTick;
            newEntry.Name = Utils::ExtractProcessName(pInfo, pid);

            newEntry.Description = NativeProcess::GetProcessDescription(handle.get());
            newEntry.Icon = std::move(Win32Helper::ProcessHelper::GetProcessIcon(handle.get()));

            auto& insertedEntry = (processCache_[pid] = std::move(newEntry));

            pi.Name = insertedEntry.Name.c_str();
            pi.Description = insertedEntry.Description.c_str();
            pi.Icon = insertedEntry.Icon.get();
            pi.IsEfficiencyModeEnabled = insertedEntry.IsEfficiencyModeEnabled;
        }*/
        return pi;
    }

    void ProcessInformationProvider::cleanupCache(std::unordered_set<uint32_t> const& currentTickPids)
    {
        std::erase_if(processCache_, [&currentTickPids](auto& pair)
        {
            if (!pair.second.IsActive) return true; 

            if (!currentTickPids.contains(pair.first)) {
                pair.second.IsActive = false; 
            }
            return false;
        });
    }
}