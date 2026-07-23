#include "pch.h"
#include "ProcessInformationProvider.h"
#include "Native.h"
#include "N2/DeltaUtils.h"
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

    std::vector<native::shared_process_item> ProcessInformationProvider::GetAllProcesses()
    {
        auto lock = lock_.lock_shared();
        return activeProcesses_;
    }

    native::shared_process_item ProcessInformationProvider::GetProcess(uint32_t pid)
    {
        auto lock = lock_.lock_shared();

        auto it = std::ranges::find(activeProcesses_, pid, [](const auto& proc)
        {
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
      /*  auto currentSystemTime = NativeSystem::GetCurrentSystemTime();
        auto currentTick = GetTickCount64();

        auto buffer = native::nt_safe_wrapper([](auto buffer, auto size, auto returnLength)
        {
            return NtQuerySystemInformation(SystemProcessInformation, buffer, size, returnLength);
        });

        std::vector<native::shared_process_item> newActiveProcesses;
        absl::flat_hash_set<HANDLE> currentTickPids;

        currentTickPids.reserve(processCache_.size());

        auto* pHead = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(buffer.get());

        for (auto& pInfo : native::nt_make_range(pHead))
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
        processCache_.purge_inactive(currentTickPids);*/
        //dataSource_->QueryProcesses();
    }

    native::shared_process_item ProcessInformationProvider::parseCacheProcess(
        _In_ PSYSTEM_PROCESS_INFORMATION Process,
        IN HANDLE pid,
        IN uint64_t currentSystemTime,
        IN uint64_t currentTick
    )
    {
       /* native::shared_process_item item = processCache_.find_or_create(
            pid,
            [Process](const native::shared_process_item& existingItem) {
                return existingItem->CreateTime.QuadPart == Process->CreateTime.QuadPart;
            },
            [pid, Process]() {
                auto newItem = std::shared_ptr<SE_PROCESS_ITEM>(new SE_PROCESS_ITEM{}, native::details::process_item_deleter{});

                newItem->ProcessId = pid;
                newItem->ParentProcessId = reinterpret_cast<HANDLE>(Process->InheritedFromUniqueProcessId);
                newItem->CreateTime = Process->CreateTime;
                newItem->SessionId = Process->SessionId;
                newItem->BasePriority = Process->BasePriority;

                auto extractedName = static_cast<PWCH>(Process->ImageName.Buffer);
                if (extractedName)
                {
                    newItem->ProcessName = _wcsdup(extractedName);
                }

                SeOpenProcess(&newItem->QueryHandle, PROCESS_QUERY_LIMITED_INFORMATION, pid);

                bool iconSet = false;
                if (newItem->QueryHandle)
                {
                    PWSTR buffer{ nullptr };

                    if (NT_SUCCESS(SeGetProcessImageFileNameWin32(newItem->QueryHandle, &buffer)) && buffer)
                    {
                        newItem->FileName = _wcsdup(buffer);
                        newItem->SmallIconIndex = ShellHelper::GetIconIndex(newItem->FileName);
                        iconSet = true;
                        free(buffer);
                    }
                }
                if (!iconSet)
                {
                    newItem->SmallIconIndex = ShellHelper::GetDefaultIconIndex();
                }

                newItem->IoReadDelta.Value = Process->ReadTransferCount.QuadPart;
                newItem->IoWriteDelta.Value = Process->WriteTransferCount.QuadPart;
                newItem->IoOtherDelta.Value = Process->OtherTransferCount.QuadPart;
                newItem->IoReadCountDelta.Value = Process->ReadOperationCount.QuadPart;
                newItem->IoWriteCountDelta.Value = Process->WriteOperationCount.QuadPart;
                newItem->IoOtherCountDelta.Value = Process->OtherOperationCount.QuadPart;
                newItem->CpuKernelDelta.Value = Process->KernelTime.QuadPart;
                newItem->CpuUserDelta.Value = Process->UserTime.QuadPart;

                return newItem;
            }
        );

        item->State = 1;
        item->NumberOfThreads = Process->NumberOfThreads;
        item->NumberOfHandles = Process->HandleCount;
        item->WorkingSetPrivateSize = Process->PrivatePageCount;
        item->PeakNumberOfThreads = max(item->PeakNumberOfThreads, Process->NumberOfThreads);
        item->VmCounters = *(PVM_COUNTERS_EX)&Process->PeakVirtualSize;
        item->IoCounters = (Process->ReadOperationCount.QuadPart > 0) ? *reinterpret_cast<PIO_COUNTERS>(&Process->ReadOperationCount) : IO_COUNTERS{};

        native::deltamgr::Update(&item->CpuKernelDelta, Process->KernelTime.QuadPart);
        native::deltamgr::Update(&item->CpuUserDelta, Process->UserTime.QuadPart);

        item->KernelTime = Process->KernelTime;
        item->UserTime = Process->UserTime;

        native::deltamgr::Update(&item->IoReadDelta, Process->ReadTransferCount.QuadPart);
        native::deltamgr::Update(&item->IoWriteDelta, Process->WriteTransferCount.QuadPart);
        native::deltamgr::Update(&item->IoOtherDelta, Process->OtherTransferCount.QuadPart);
        native::deltamgr::Update(&item->IoReadCountDelta, Process->ReadOperationCount.QuadPart);
        native::deltamgr::Update(&item->IoWriteCountDelta, Process->WriteOperationCount.QuadPart);
        native::deltamgr::Update(&item->IoOtherCountDelta, Process->OtherOperationCount.QuadPart);
        native::deltamgr::Update(&item->PageFaultsDelta, Process->PageFaultCount);

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
        }*/

        return std::shared_ptr<SE_PROCESS_ITEM>(new SE_PROCESS_ITEM{}, native::details::process_item_deleter{});
    }
}