#include "pch.h"
#include "ProcessManager.h"
#include "Utils.h"
#include "native.h"
#include "Native/process.h"

namespace winrt::SystemExplorer::Core::System
{
	void ProcessManager::Terminate(uint32_t pid)
	{
		if (pid <= 4) return;
		
		nt::unique_nt_handle handle;
		THROW_IF_NTSTATUS_FAILED(SeOpenProcess(
			&handle,
			PROCESS_TERMINATE,
			(HANDLE)pid
		));
		THROW_IF_NTSTATUS_FAILED(NtTerminateProcess(
			handle.get(),
			NULL
		));
	}

	void ProcessManager::Suspend(uint32_t pid)
	{
		nt::unique_nt_handle handle;
		THROW_IF_NTSTATUS_FAILED(SeOpenProcess(
			&handle,
			PROCESS_TERMINATE,
			(HANDLE)pid
		));
		THROW_IF_NTSTATUS_FAILED(NtSuspendProcess(
			handle.get()
		));
	}

	void ProcessManager::Resume(uint32_t pid)
	{
		nt::unique_nt_handle handle;
		THROW_IF_NTSTATUS_FAILED(SeOpenProcess(
			&handle,
			PROCESS_TERMINATE,
			(HANDLE)pid
		));
		THROW_IF_NTSTATUS_FAILED(NtResumeProcess(
			handle.get()
		));
	}

	void ProcessManager::Restart(uint32_t pid)
	{
		auto handle = nt::unique_nt_handle{ OpenProcess(
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
		nt::unique_nt_handle handle;
		THROW_IF_NTSTATUS_FAILED(SeOpenProcess(
			&handle,
			PROCESS_TERMINATE,
			(HANDLE)pid
		));

		THROW_IF_NTSTATUS_FAILED(SeSetProcessPowerThrottlingState(
			handle.get(),
			POWER_THROTTLING_PROCESS_EXECUTION_SPEED,
			POWER_THROTTLING_PROCESS_EXECUTION_SPEED
		));
	}
	void ProcessManager::DisableEfficiencyMode(uint32_t pid)
	{
		nt::unique_nt_handle handle;
		THROW_IF_NTSTATUS_FAILED(SeOpenProcess(
			&handle,
			PROCESS_TERMINATE,
			(HANDLE)pid
		));

		THROW_IF_NTSTATUS_FAILED(SeSetProcessPowerThrottlingState(
			handle.get(),
			POWER_THROTTLING_PROCESS_EXECUTION_SPEED,
			POWER_THROTTLING_PROCESS_EXECUTION_SPEED
		));
	}
}