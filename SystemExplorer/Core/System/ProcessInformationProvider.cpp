#include "pch.h"
#include "ProcessInformationProvider.h"
#include <Helpers/Win32Helper.h>
#include <unordered_set>

using namespace winrt::SystemExplorer::Helpers;

static constexpr ULONG INITIAL_BUFFER_SIZE{ 6400 };
static constexpr ULONG MAX_BUFFER_SIZE{ 1280000 };

namespace winrt::SystemExplorer::Core::System
{
    ProcessInformationProvider::ProcessInformationProvider()
    {
        bufferSize_ = INITIAL_BUFFER_SIZE;

        thread_ = std::make_unique<ProviderThread>(
            [this]() { this->updateProcesses(); },
            std::chrono::milliseconds(1000)
        );
    }

    std::vector<PROCESS_INFORMATION> ProcessInformationProvider::GetAllProcesses()
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

        auto currentSystemTime =
            (ULARGE_INTEGER{ kernelTime.dwLowDateTime, kernelTime.dwHighDateTime }.QuadPart) +
            (ULARGE_INTEGER{ userTime.dwLowDateTime, userTime.dwHighDateTime }.QuadPart);

        auto currentTick = GetTickCount64();
        ULONG returnLength{ 0 };
        NTSTATUS status;

        do
        {
            buffer_.reset(static_cast<PBYTE>(
                VirtualAlloc(NULL, bufferSize_, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)
                ));
            THROW_IF_NULL_ALLOC(buffer_);

            status = NtQuerySystemInformation(
                SystemProcessInformation,
                buffer_.get(),
                bufferSize_,
                &returnLength
            );

            if (status == STATUS_INFO_LENGTH_MISMATCH || status == STATUS_BUFFER_OVERFLOW) [[likely]]
            {
                if (returnLength > 0) [[likely]]
                    bufferSize_ = returnLength;
                else
                    bufferSize_ = min(bufferSize_ * 2, MAX_BUFFER_SIZE);
                continue;
            }
            if (bufferSize_ >= MAX_BUFFER_SIZE) [[unlikely]]
                THROW_HR(E_OUTOFMEMORY);

        } while (status == STATUS_INFO_LENGTH_MISMATCH);

        THROW_IF_FAILED_MSG(status, "QuerySystemInformation failed");

        auto* pInfo = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(buffer_.get());

        std::vector<PROCESS_INFORMATION> newActiveProcesses;
        std::unordered_set<uint32_t> currentTickPids;

        while (true)
        {
            uint32_t pid = static_cast<uint32_t>(reinterpret_cast<ULONG_PTR>(pInfo->UniqueProcessId));
            currentTickPids.insert(pid);

            PROCESS_INFORMATION pi{};
            pi.Pid = pid;
            pi.ParentId = static_cast<uint32_t>(reinterpret_cast<ULONG_PTR>(pInfo->InheritedFromUniqueProcessId));
            pi.PrivateBytes = static_cast<uint32_t>(pInfo->PrivatePageCount);
            pi.CpuUsage = 0.0f;
            pi.IoRate = 0;

            auto currentIoCount = pInfo->ReadTransferCount.QuadPart +
                pInfo->WriteTransferCount.QuadPart +
                pInfo->OtherTransferCount.QuadPart;

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
            }
            else
            {
                ProcessCacheEntry newEntry{};
                newEntry.CreateTime = pInfo->CreateTime;
                newEntry.LastSystemTime = currentSystemTime;
                newEntry.LastProcessTime = currentProcessTime;
                newEntry.LastIoTransferCount = currentIoCount;
                newEntry.LastTickCount = currentTick;
                newEntry.IsActive = true;

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
                auto& insertedEntry = (processCache_[pid] = std::move(newEntry));

                pi.Name = insertedEntry.Name.c_str();
                pi.Description = insertedEntry.Description.c_str();
            }

            newActiveProcesses.push_back(pi);

            if (pInfo->NextEntryOffset == 0) {
                break;
            }
            pInfo = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(
                reinterpret_cast<PBYTE>(pInfo) + pInfo->NextEntryOffset);
        }
        {
            auto lock = lock_.lock_exclusive();
            activeProcesses_ = std::move(newActiveProcesses);
        }
        std::erase_if(processCache_, [](const auto& pair)
        {
            return !pair.second.IsActive;
        });
        for (auto& pair : processCache_)
        {
            if (!currentTickPids.contains(pair.first))
            {
                pair.second.IsActive = false;
            }
        }
    }
}