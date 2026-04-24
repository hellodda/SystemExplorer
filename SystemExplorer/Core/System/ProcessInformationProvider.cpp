#include "pch.h"
#include "ProcessInformationProvider.h"

#include <phnt_windows.h>
#include <phnt.h>

#include <Helpers/Win32Helper.h>

using namespace winrt::SystemExplorer::Helpers;

namespace winrt::SystemExplorer::Core::System
{
    ProcessInformationProvider::ProcessInformationProvider()
    {
        bufferSize_ = 1024 * 1024;
        buffer_ = std::make_unique<uint8_t[]>(bufferSize_);

        thread_ = std::make_unique<ProviderThread>(
            [this]() { this->updateProcesses(); },
            std::chrono::milliseconds(1000)
        );
    }

    std::vector<ProcessInformation> ProcessInformationProvider::GetAllProcesses()
    {
        auto lock = lock_.lock_shared();
        return activeProcesses_;
    }

    IProviderThread* ProcessInformationProvider::Thread()
    {
        return thread_.get();
    }

    void ProcessInformationProvider::updateProcesses()
    {
        FILETIME idleTime, kernelTime, userTime;
        GetSystemTimes(&idleTime, &kernelTime, &userTime);

        ULONGLONG currentSystemTime =
            (ULARGE_INTEGER{ kernelTime.dwLowDateTime, kernelTime.dwHighDateTime }.QuadPart) +
            (ULARGE_INTEGER{ userTime.dwLowDateTime, userTime.dwHighDateTime }.QuadPart);

        uint64_t currentTick = GetTickCount64();

        ULONG returnLength{ 0 };
        NTSTATUS status;

        do
        {
            status = NtQuerySystemInformation(
                SystemProcessInformation,
                buffer_.get(),
                bufferSize_,
                &returnLength);

            if (status == STATUS_INFO_LENGTH_MISMATCH)
            {
                bufferSize_ = returnLength + (1024 * 1024);
                buffer_ = std::make_unique<uint8_t[]>(bufferSize_);
            }
        } while (status == STATUS_INFO_LENGTH_MISMATCH);

        if (!NT_SUCCESS(status))
        {
            return;
        }

        auto* pInfo = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(buffer_.get());

        std::unordered_map<uint32_t, ProcessCacheEntry> newCache;
        std::vector<ProcessInformation> newActiveProcesses;

        while (true)
        {
            uint32_t pid = static_cast<uint32_t>(reinterpret_cast<ULONG_PTR>(pInfo->UniqueProcessId));

            ProcessInformation proc{};
            proc.Pid = pid;
            proc.ParentId = static_cast<uint32_t>(reinterpret_cast<ULONG_PTR>(pInfo->InheritedFromUniqueProcessId));
            proc.PrivateBytes = static_cast<uint32_t>(pInfo->PrivatePageCount);

            ULONGLONG currentIoCount = pInfo->ReadTransferCount.QuadPart +
                pInfo->WriteTransferCount.QuadPart +
                pInfo->OtherTransferCount.QuadPart;

            ULONGLONG currentProcessTime = pInfo->KernelTime.QuadPart + pInfo->UserTime.QuadPart;

            auto it = processCache_.find(pid);
            if (it != processCache_.end() && it->second.CreateTime.QuadPart == pInfo->CreateTime.QuadPart)
            {
                ProcessCacheEntry& cache = it->second;
                proc.Name = cache.Name;
                proc.Description = cache.Description;

                ULONGLONG sysDelta = currentSystemTime - cache.LastSystemTime;
                ULONGLONG procDelta = currentProcessTime - cache.LastProcessTime;

                if (sysDelta > 0)
                {
                    float rawCpu = static_cast<float>(procDelta * 100.0 / sysDelta);
                    proc.CpuUsage = std::round(rawCpu * 10.0f) / 10.0f;
                }

                uint64_t tickDelta = currentTick - cache.LastTickCount;
                if (tickDelta > 0)
                {
                    ULONGLONG ioDelta = currentIoCount - cache.LastIoTransferCount;
                    proc.IoRate = static_cast<uint32_t>((ioDelta * 1000ULL) / tickDelta);
                }

                cache.LastSystemTime = currentSystemTime;
                cache.LastProcessTime = currentProcessTime;
                cache.LastIoTransferCount = currentIoCount;
                cache.LastTickCount = currentTick;

                newCache[pid] = cache;
            }
            else
            {
                ProcessCacheEntry newEntry{};
                newEntry.CreateTime = pInfo->CreateTime;
                newEntry.LastSystemTime = currentSystemTime;
                newEntry.LastProcessTime = currentProcessTime;
                newEntry.LastIoTransferCount = currentIoCount;
                newEntry.LastTickCount = currentTick;

                if (pInfo->ImageName.Buffer != nullptr)
                {
                    newEntry.Name = hstring(pInfo->ImageName.Buffer, pInfo->ImageName.Length / sizeof(WCHAR));
                }
                else if (pid == 0)
                {
                    newEntry.Name = L"Idle";
                }
                else
                {
                    newEntry.Name = L"System";
                }

                newEntry.Description = Win32Helper::ProcessHelper::GetProcessDescription(pid);

                proc.Name = newEntry.Name;
                proc.Description = newEntry.Description;
                proc.CpuUsage = 0.0f;
                proc.IoRate = 0;

                newCache[pid] = newEntry;
            }

            newActiveProcesses.push_back(std::move(proc));

            if (pInfo->NextEntryOffset == 0) {
                break;
            }
            pInfo = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(
                reinterpret_cast<uint8_t*>(pInfo) + pInfo->NextEntryOffset);
        }
        {
            auto lock = lock_.lock_exclusive();
            activeProcesses_ = std::move(newActiveProcesses);
        }
        processCache_ = std::move(newCache);
    }
}