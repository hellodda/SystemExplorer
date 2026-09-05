#pragma once
#include "MonitorBase.h"

#include <core/eil/event.h>

#include "../sysx.h"
#include "../Sources/IProcessDataSource.h"

namespace winrt::SystemExplorer::Core::System::Monitors
{
    struct ProcessMonitor : MonitorBase
    {
        explicit ProcessMonitor(MonitorOptions const& options)
            : MonitorBase(options) {}

        eil::event<
            eil::action_t<std::span<PSYSX_PROCESS_ITEM>>
        > OnDataCollected;

        [[nodiscard]] HRESULT DataSource(std::unique_ptr<Sources::IProcessDataSource> dataSource) noexcept;

    private:
        void OnTimer() override;
        void OnStart() override;
        void OnSuspend() override;
        void OnStop() override;
        void raiseDataCollected();

    private:
        std::unique_ptr<Sources::IProcessDataSource> dataSource_;

        std::vector<SYSX_PROCESS_ITEM> rawSnapshotBuffer_;
        std::vector<PSYSX_PROCESS_ITEM> viewBuffer_;

        absl::flat_hash_map<HANDLE, SYSX_PROCESS_ITEM> cache_;
        absl::flat_hash_set<HANDLE> currentPids_;

        uint64_t lastSystemTime_{ 0 };
    };
}