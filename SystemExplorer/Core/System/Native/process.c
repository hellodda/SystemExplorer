#include "process.h"
#include <malloc.h>

NTSTATUS SeOpenProcess(
	_Out_ PHANDLE ProcessHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_ HANDLE ProcessId
)
{
	NTSTATUS status;
	OBJECT_ATTRIBUTES objectAttributes;
	CLIENT_ID clientId;

	clientId.UniqueProcess = ProcessId;
	clientId.UniqueThread = NULL;

	InitializeObjectAttributes(&objectAttributes, NULL, 0, NULL, NULL);
	status = NtOpenProcess(
		ProcessHandle,
		DesiredAccess,
		&objectAttributes,
		&clientId
	);
	return status;
}

NTSTATUS SeOpenProcessToken(
	_In_ HANDLE ProcessHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_Out_ PHANDLE TokenHandle
)
{
	NTSTATUS status;

	status = NtOpenProcessToken(
		ProcessHandle,
		DesiredAccess,
		TokenHandle
	);
	return status;
}


NTSTATUS SeGetProcessPowerThrottlingState(
	_In_ HANDLE ProcessHandle,
	_Out_ PPOWER_THROTTLING_PROCESS_STATE PowerThrottlingState
)
{
	NTSTATUS status;

	memset(PowerThrottlingState, 0, sizeof(POWER_THROTTLING_PROCESS_STATE));
	PowerThrottlingState->Version = POWER_THROTTLING_PROCESS_CURRENT_VERSION;

	status = NtQueryInformationProcess(
		ProcessHandle,
		ProcessPowerThrottlingState,
		PowerThrottlingState,
		sizeof(POWER_THROTTLING_PROCESS_STATE),
		NULL
	);

	return status;
}

NTSTATUS SeSetProcessPowerThrottlingState(
	_In_ HANDLE ProcessHandle,
	_In_ ULONG ControlMask,
	_In_ ULONG StateMask
)
{
	NTSTATUS status;
	POWER_THROTTLING_PROCESS_STATE powerThrottlingState;

	memset(&powerThrottlingState, 0, sizeof(PPROCESS_POWER_THROTTLING_STATE));
	powerThrottlingState.Version = POWER_THROTTLING_PROCESS_CURRENT_VERSION;
	powerThrottlingState.ControlMask = ControlMask;
	powerThrottlingState.StateMask = StateMask;

	status = NtSetInformationProcess(
		ProcessHandle,
		ProcessPowerThrottlingState,
		&powerThrottlingState,
		sizeof(POWER_THROTTLING_PROCESS_STATE)
	);
	return status;
}

NTSTATUS SeGetProcessProtection(
	_In_ HANDLE ProcessHandle,
	_Out_ PPS_PROTECTION Protection
)
{
	return NtQueryInformationProcess(
		ProcessHandle,
		ProcessProtectionInformation,
		Protection,
		sizeof(PS_PROTECTION),
		NULL
	);
}

NTSTATUS SeSuspendProcess(
	_In_ HANDLE ProcessHandle
)
{
	NTSTATUS status;

	status = NtSuspendProcess(
		ProcessHandle
	);

	return status;
}

NTSTATUS SeResumeProcess(
	_In_ HANDLE ProcessHandle
)
{
	NTSTATUS status;

	status = NtResumeProcess(
		ProcessHandle
	);

	return status;
}

NTSTATUS SeGetProcessBasicInformation(
	_In_ HANDLE ProcessHandle,
	_Out_ PPROCESS_BASIC_INFORMATION BasicInformation
)
{
	return NtQueryInformationProcess(
		ProcessHandle,
		ProcessBasicInformation,
		BasicInformation,
		sizeof(PROCESS_BASIC_INFORMATION),
		NULL
	);
}

NTSTATUS SeGetProcessExtendedBasicInformation(
	_In_ HANDLE ProcessHandle,
	_Out_ PPROCESS_EXTENDED_BASIC_INFORMATION ExtendedBasicInformation
)
{
	ExtendedBasicInformation->Size = sizeof(PROCESS_EXTENDED_BASIC_INFORMATION);

	return NtQueryInformationProcess(
		ProcessHandle,
		ProcessBasicInformation,
		ExtendedBasicInformation,
		sizeof(PROCESS_EXTENDED_BASIC_INFORMATION),
		NULL
	);
}
NTSTATUS SeGetProcessImageFileNameWin32(
	_In_ HANDLE ProcessHandle,
	_Out_ PWSTR* FileName
)
{
	NTSTATUS status;
	PUNICODE_STRING fileName;
	ULONG bufferLength;
	ULONG returnLength = 0;

	*FileName = NULL;

	bufferLength = sizeof(UNICODE_STRING) + DOS_MAX_PATH_LENGTH * sizeof(WCHAR);

	fileName = _malloca(bufferLength);
	if (!fileName)
		return STATUS_NO_MEMORY;

	status = NtQueryInformationProcess(
		ProcessHandle,
		ProcessImageFileNameWin32,
		fileName,
		bufferLength,
		&returnLength
	);

	if (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		_freea(fileName);

		bufferLength = returnLength;

		fileName = _malloca(bufferLength);
		if (!fileName)
			return STATUS_NO_MEMORY;

		status = NtQueryInformationProcess(
			ProcessHandle,
			ProcessImageFileNameWin32, 
			fileName,
			bufferLength,
			&returnLength
		);
	}

	if (NT_SUCCESS(status))
	{
		SIZE_T size;

		if (RtlIsNullOrEmptyUnicodeString(fileName))
		{
			_freea(fileName);
			return STATUS_UNSUCCESSFUL;
		}

		size = fileName->Length + sizeof(WCHAR);

		*FileName = malloc(size);

		if (!*FileName)
		{
			_freea(fileName);
			return STATUS_NO_MEMORY;
		}

		memcpy(*FileName, fileName->Buffer, fileName->Length);
		(*FileName)[fileName->Length / sizeof(WCHAR)] = UNICODE_NULL;
	}

	_freea(fileName);

	return status;
}

NTSTATUS SeGetProcessMitigationPolicy(
	_In_ HANDLE ProcessHandle,
	_In_ PROCESS_MITIGATION_POLICY Policy,
	_Out_ PPROCESS_MITIGATION_POLICY_INFORMATION MitigationPolicy
)
{
	memset(MitigationPolicy, 0, sizeof(PROCESS_MITIGATION_POLICY_INFORMATION));
	MitigationPolicy->Policy = Policy;

	return NtQueryInformationProcess(
		ProcessHandle,
		ProcessMitigationPolicy,
		MitigationPolicy,
		sizeof(PROCESS_MITIGATION_POLICY_INFORMATION),
		NULL
	);
}

NTSTATUS SeIsProcessEfficiencyModeEnabled(
	_In_ HANDLE ProcessHandle,
	_Out_ PBOOLEAN EfficiencyMode
)
{
	NTSTATUS status;
	PROCESS_POWER_THROTTLING_STATE powerThrottlingState;
	powerThrottlingState.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;

	status = SeGetProcessPowerThrottlingState(ProcessHandle, &powerThrottlingState);

	if (NT_SUCCESS(status))
	{
		*EfficiencyMode = (powerThrottlingState.ControlMask & PROCESS_POWER_THROTTLING_EXECUTION_SPEED) &&
			(powerThrottlingState.StateMask & PROCESS_POWER_THROTTLING_EXECUTION_SPEED);
	}
	*EfficiencyMode = FALSE;

	return status;
}
NTSTATUS SeGetProcessErrorMode(
	_In_ HANDLE ProcessHandle,
	_Out_ PULONG ErrorMode
)
{
	return NtQueryInformationProcess(
		ProcessHandle,
		ProcessDefaultHardErrorMode,
		ErrorMode,
		sizeof(ULONG),
		NULL
	);
}

NTSTATUS SeSetProcessErrorMode(
	_In_ HANDLE ProcessHandle,
	_In_ ULONG ErrorMode
)
{
	return NtSetInformationProcess(
		ProcessHandle,
		ProcessDefaultHardErrorMode,
		&ErrorMode,
		sizeof(ULONG)
	);
}

NTSTATUS SeGetProcessIsWow64(
	_In_ HANDLE ProcessHandle,
	_Out_ PBOOLEAN IsWow64Process
)
{
	NTSTATUS status;
	ULONG_PTR wow64;

	status = NtQueryInformationProcess(
		ProcessHandle,
		ProcessWow64Information,
		&wow64,
		sizeof(ULONG_PTR),
		NULL
	);

	if (NT_SUCCESS(status))
	{
		*IsWow64Process = !!wow64;
	}

	return status;
}