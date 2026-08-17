#pragma once
#include "MonitorBase.h"

#include <Core/Eil/event.h>
#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>
#include <absl/container/node_hash_map.h> 
#include <span>

#include "../Sources/IProcessDataSource.h"

namespace winrt::SystemExplorer::Core::System::Monitors
{
	struct ProcessMonitor : MonitorBase
	{
		ProcessMonitor(MonitorOptions const& options)
			: MonitorBase(options) {
		}

		eil::fast_event<
			eil::faction_t<std::span<SYSX_PROCESS_ITEM*>>
		> OnDataCollected;

		[[nodiscard]] HRESULT DataSource(std::unique_ptr<Sources::IProcessDataSource> dataSource) noexcept;

	private:
		void OnTimer() override;
		void OnStart() override;
		void OnSuspend() override;
		void OnStop() override;
		void raiseDataCollected();
	private:
		absl::node_hash_map<DWORD, SYSX_PROCESS_ITEM> cache_;
		std::vector<SYSX_PROCESS_ITEM> rawBuffer_;
		std::vector<SYSX_PROCESS_ITEM*> viewBuffer_;
		std::unique_ptr<Sources::IProcessDataSource> dataSource_;
	};
}