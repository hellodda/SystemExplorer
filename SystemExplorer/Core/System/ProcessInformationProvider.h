#pragma once
#include "System.h"
#include "ProviderThread.h"
#include <unordered_set>
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

        std::vector<ProcessNativeInformation> GetAllProcesses() override;

        ProviderThread& Thread() override;

    private:
        void updateProcesses();
        
        ProcessNativeInformation parseCacheProcess(
            _In_ PSYSTEM_PROCESS_INFORMATION pInfo,
            IN uint32_t pid,
            IN uint64_t currentSystemTime,
            IN uint64_t currentTick
        );
        
        void cleanupCache(std::unordered_set<uint32_t> const& currentTickPids);
    private:
        std::shared_ptr<ProviderRegistration> registry_;
        ProviderThread thread_;

        wil::srwlock lock_;
        std::vector<ProcessNativeInformation> activeProcesses_;
        std::unordered_map<uint32_t, ProcessCacheEntry> processCache_;
    };
}