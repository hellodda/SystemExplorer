#include "pch.h"
#include "ProcessInformationProvider.h"

#include <unordered_set>
#include "Native.h"
#include <Helpers/Win32/Native/NativeProcess.h>
#include <Helpers/Win32/Native/NativeSystem.h>
#include <Helpers/Win32/ShellHelper.h>

using namespace winrt::SystemExplorer::Helpers;
using namespace winrt::SystemExplorer::Helpers::Win32;
using namespace winrt::SystemExplorer::Helpers::Win32::Native;

namespace winrt::SystemExplorer::Core::System
{
    ProcessInformationProvider::ProcessInformationProvider()
    {
        registry_ = thread_.Register([this]() {
            updateProcesses();
        });
        registry_->Enable();
        thread_.Start(std::chrono::milliseconds(1000));
    }

    ProcessInformationProvider::~ProcessInformationProvider()
    {
        auto lock = lock_.lock_exclusive();
        for (auto& [pid, item] : processCache_)
        {
            if (item->ProcessName) free((void*)item->ProcessName);
            if (item->FileName) free((void*)item->FileName);
            if (item->CommandLine) free((void*)item->CommandLine);
            if (item->QueryHandle) CloseHandle(item->QueryHandle);
        }
    }

    std::vector<PSE_PROCESS_ITEM> ProcessInformationProvider::GetAllProcesses()
    {
        auto lock = lock_.lock_shared();
        return activeProcesses_;
    }

    PSE_PROCESS_ITEM ProcessInformationProvider::GetProcess(uint32_t pid)
    {
        auto lock = lock_.lock_shared();

        auto it = std::ranges::find(activeProcesses_, pid, [](const auto& proc) {
            return static_cast<uint32_t>(reinterpret_cast<ULONG_PTR>(proc->ProcessId));
        });

        return it != activeProcesses_.end() ? *it : nullptr;
    }

    ProviderThread& ProcessInformationProvider::Thread()
    {
        return thread_;
    }

    void ProcessInformationProvider::updateProcesses()
    {
        auto currentSystemTime = NativeSystem::GetCurrentSystemTime();
        auto currentTick = GetTickCount64();

        auto buffer = nt::nt_safe_wrapper([](auto buffer, auto size, auto returnLength)
        {
            return NtQuerySystemInformation(SystemProcessInformation, buffer, size, returnLength);
        });

        std::vector<PSE_PROCESS_ITEM> newActiveProcesses;
        std::unordered_set<HANDLE> currentTickPids;

        auto* pHead = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(buffer.get());

        for (auto& pInfo : nt::nt_make_range(pHead))
        {
            HANDLE pid = reinterpret_cast<HANDLE>(pInfo.UniqueProcessId);
            currentTickPids.insert(pid);

            newActiveProcesses.push_back(parseCacheProcess(&pInfo, pid, currentSystemTime, currentTick));
        }

        {
            auto lock = lock_.lock_exclusive();
            activeProcesses_ = std::move(newActiveProcesses);
            lastSystemTime_ = currentSystemTime;
        }

        cleanupCache(currentTickPids);
    }

    PSE_PROCESS_ITEM ProcessInformationProvider::parseCacheProcess(
        _In_ PSYSTEM_PROCESS_INFORMATION Process,
        IN HANDLE pid,
        IN uint64_t currentSystemTime,
        IN uint64_t currentTick
    )
    {
        auto it = processCache_.find(pid);
        SE_PROCESS_ITEM* item{ nullptr };

        if (it != processCache_.end() && it->second->CreateTime.QuadPart == Process->CreateTime.QuadPart)
        {
            item = it->second.get();
        }
        else
        {
            auto newItem = std::make_unique<SE_PROCESS_ITEM>();
            RtlZeroMemory(newItem.get(), sizeof(SE_PROCESS_ITEM));
            item = newItem.get();

            item->ProcessId = pid;
            item->ParentProcessId = reinterpret_cast<HANDLE>(Process->InheritedFromUniqueProcessId);
            item->CreateTime = Process->CreateTime;
            item->SessionId = Process->SessionId;
            item->BasePriority = Process->BasePriority;

            auto extractedName = static_cast<PWCH>(Process->ImageName.Buffer);
            if (extractedName)
            {
                item->ProcessName = _wcsdup(extractedName);
            }

            SeOpenProcess(&item->QueryHandle,
                PROCESS_QUERY_LIMITED_INFORMATION,
                pid
            );

            bool iconSet = false;
            if (item->QueryHandle)
            {
                PWSTR buffer{ nullptr };

                if (NT_SUCCESS(SeGetProcessImageFileNameWin32(item->QueryHandle, &buffer)) && buffer)
                {
                    item->FileName = _wcsdup(buffer); 

                    item->SmallIconIndex = ShellHelper::GetIconIndex(item->FileName);
                    
                    iconSet = true;

                    free(buffer);
                }
            }
            if (!iconSet)
            {
                item->SmallIconIndex = ShellHelper::GetDefaultIconIndex();
            }


            item->IoReadDelta.Value = Process->ReadTransferCount.QuadPart;
            item->IoWriteDelta.Value = Process->WriteTransferCount.QuadPart;
            item->IoOtherDelta.Value = Process->OtherTransferCount.QuadPart;
            item->IoReadCountDelta.Value = Process->ReadOperationCount.QuadPart;
            item->IoWriteCountDelta.Value = Process->WriteOperationCount.QuadPart;
            item->IoOtherCountDelta.Value = Process->OtherOperationCount.QuadPart;

            item->CpuKernelDelta.Value = Process->KernelTime.QuadPart;
            item->CpuUserDelta.Value = Process->UserTime.QuadPart;

            processCache_[pid] = std::move(newItem);
        }

        item->State = 1;
        item->NumberOfThreads = Process->NumberOfThreads;
        item->NumberOfHandles = Process->HandleCount;
        item->WorkingSetPrivateSize = Process->PrivatePageCount;
        item->PeakNumberOfThreads = max(item->PeakNumberOfThreads, Process->NumberOfThreads);
        item->VmCounters = *(PVM_COUNTERS_EX)&Process->PeakVirtualSize;
        item->IoCounters = *(PIO_COUNTERS)&Process->ReadOperationCount;;

        item->IoCounters = (Process->ReadOperationCount.QuadPart > 0) ? *reinterpret_cast<PIO_COUNTERS>(&Process->ReadOperationCount) : IO_COUNTERS{};

        UpdateDelta(&item->CpuKernelDelta, Process->KernelTime.QuadPart);
        UpdateDelta(&item->CpuUserDelta, Process->UserTime.QuadPart);
        item->KernelTime = Process->KernelTime;
        item->UserTime = Process->UserTime;

        UpdateDelta(&item->IoReadDelta, Process->ReadTransferCount.QuadPart);
        UpdateDelta(&item->IoWriteDelta, Process->WriteTransferCount.QuadPart);
        UpdateDelta(&item->IoOtherDelta, Process->OtherTransferCount.QuadPart);
        UpdateDelta(&item->IoReadCountDelta, Process->ReadOperationCount.QuadPart);
        UpdateDelta(&item->IoWriteCountDelta, Process->WriteOperationCount.QuadPart);
        UpdateDelta(&item->IoOtherCountDelta, Process->OtherOperationCount.QuadPart);
        UpdateDelta32(&item->PageFaultsDelta, Process->PageFaultCount);

        auto sysDelta = currentSystemTime - lastSystemTime_;
        if (sysDelta > 0 && lastSystemTime_ > 0)
        {
            auto procDelta = item->CpuKernelDelta.Delta + item->CpuUserDelta.Delta;
            auto rawCpu = static_cast<FLOAT>(procDelta * 100.0 / sysDelta);
            item->CpuUsage = std::round(rawCpu * 10.0f) / 10.0f;

            item->CpuKernelUsage = static_cast<FLOAT>(item->CpuKernelDelta.Delta * 100.0 / sysDelta);
            item->CpuUserUsage = static_cast<FLOAT>(item->CpuUserDelta.Delta * 100.0 / sysDelta);
        }
        else if (lastSystemTime_ == 0)
        {
            item->CpuUsage = 0.0f;
        }

        return item;
    }

    void ProcessInformationProvider::cleanupCache(std::unordered_set<HANDLE> const& currentTickPids)
    {
        std::erase_if(processCache_, [&currentTickPids](auto& pair)
        {
            if (!currentTickPids.contains(pair.first))
            {
                SeDestroyProcessItem(pair.second.get());

                return true;
            }
            return false;
        });
    }
}