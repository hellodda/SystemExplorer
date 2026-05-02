#pragma once
#include "System.h"
#include "pthread.h"
#include <unordered_map>
#include <wil/resource.h> 

#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "version.lib")

namespace winrt::SystemExplorer::Core::System
{
    using namespace winrt::SystemExplorer::Core::System::Contracts;

    struct ProcessCacheEntry
    {
        LARGE_INTEGER CreateTime{};
        ULONGLONG LastSystemTime{};
        ULONGLONG LastProcessTime{};
        ULONGLONG LastIoTransferCount{};
        uint64_t LastTickCount{};
        uint64_t LastStatusCheckTick{ 0 };
        hstring Name;
        hstring Description;
        bool IsActive{ true };
        bool IsEfficiencyModeEnabled{ false };
        bool IsSuspended{ false };
        wil::unique_hicon Icon;
    };

    struct ProcessInformationProvider : public IProcessInformationProvider
    {
        ProcessInformationProvider();

        std::vector<PROCESS_INFORMATION> GetAllProcesses() override;

        IProviderThread* Thread() override;

    private:
        void updateProcesses();
    private:
        std::unique_ptr<IProviderThread> thread_;

        wil::srwlock lock_;
        std::vector<PROCESS_INFORMATION> activeProcesses_;
        std::unordered_map<uint32_t, ProcessCacheEntry> processCache_;

        ULONG bufferSize_;
        wil::unique_virtualalloc_ptr<BYTE> buffer_;
    };
}