#include "pch.h"
#include "ProcessManager.h"
#include "Utils.h"

namespace winrt::SystemExplorer::Core::System
{
	void ProcessManager::Terminate(uint32_t pid)
	{
		if (pid <= 4) return;

		auto handle = wil::unique_process_handle{ OpenProcess(
			PROCESS_TERMINATE,
			FALSE,
			pid
		) };

		if (!handle.is_valid())
			THROW_LAST_ERROR_MSG("Cannot open process");

		TerminateProcess(handle.get(), EXIT_SUCCESS);
	}

	void ProcessManager::Restart(uint32_t pid)
	{
		auto handle = wil::unique_process_handle{ OpenProcess(
			PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE,
			FALSE,
			pid
		) };

		if (!handle.is_valid())
			THROW_LAST_ERROR_MSG("Cannot open process");

		Utils::RestartProcess(handle.get());
	}

	void ProcessManager::EnableEfficiencyMode(uint32_t pid)
	{
		auto handle = wil::unique_process_handle{ OpenProcess(
			PROCESS_SET_INFORMATION | PROCESS_QUERY_INFORMATION,
			FALSE,
			pid
		) };

		if (!handle.is_valid())
			THROW_LAST_ERROR_MSG("Cannot open process");

		PROCESS_POWER_THROTTLING_STATE state{};
		state.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;
		state.ControlMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
		state.StateMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;

		SetProcessInformation(
			handle.get(),
			ProcessPowerThrottling,
			&state,
			sizeof(state)
		);

	}
	void ProcessManager::DisableEfficiencyMode(uint32_t pid)
	{
		auto handle = wil::unique_process_handle{ OpenProcess(
			PROCESS_SET_INFORMATION | PROCESS_QUERY_INFORMATION,
			FALSE,
			pid
		) };

		if (!handle.is_valid())
			THROW_LAST_ERROR_MSG("Cannot open process");

		PROCESS_POWER_THROTTLING_STATE state{};
		state.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;
		state.ControlMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
		state.StateMask = NULL;

		SetProcessInformation(
			handle.get(),
			ProcessPowerThrottling,
			&state,
			sizeof(state)
		);
	}
}