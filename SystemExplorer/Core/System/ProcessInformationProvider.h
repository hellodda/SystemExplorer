#pragma once
#include "System.h"
#include "ProviderThread.h"
#include "Native/procproc.h"
#include "Native.h"

#include <unordered_set>
#include <wil/resource.h> 

#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "version.lib")



namespace winrt::SystemExplorer::Core::System
{
    struct ProcessInformationProvider
    {
        ProcessInformationProvider();
        ~ProcessInformationProvider() = default;

        std::vector<native::shared_process_item> GetAllProcesses();
        native::shared_process_item GetProcess(uint32_t pid);

        ProviderThread& Thread();

    private:
        void updateProcesses();

        native::shared_process_item parseCacheProcess(
            _In_ PSYSTEM_PROCESS_INFORMATION pInfo,
            IN HANDLE pid,
            IN uint64_t currentSystemTime,
            IN uint64_t currentTick
        );

        void cleanupCache(std::unordered_set<HANDLE> const& currentTickPids);

        inline void UpdateDelta(PSE_UINT64_DELTA DeltaStruct, ULONGLONG NewValue)
        {
            DeltaStruct->Delta = NewValue - DeltaStruct->Value;
            DeltaStruct->Value = NewValue;
        }

        inline void UpdateDelta32(PSE_UINT32_DELTA DeltaStruct, ULONG NewValue)
        {
            DeltaStruct->Delta = NewValue - DeltaStruct->Value;
            DeltaStruct->Value = NewValue;
        }

    private:
        std::shared_ptr<ProviderRegistration> registry_;
        ProviderThread thread_;

        wil::srwlock lock_;

        std::vector<native::shared_process_item> activeProcesses_;
        std::unordered_map<HANDLE, native::shared_process_item> processCache_;

        uint64_t lastSystemTime_{ 0 };
    };
}