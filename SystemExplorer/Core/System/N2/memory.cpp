#include "pch.h"
#include "memory.h"

NTSTATUS SYSX_IMPL_GetSystemBasicInformation(
	_In_opt_ PSYSTEM_BASIC_INFORMATION information
)
{
	return NtQuerySystemInformation(
		SystemBasicInformation,
		information,
		sizeof(SYSTEM_BASIC_INFORMATION),
		NULL
	);
}

NTSTATUS SYSX_IMPL_GetSystemPerformanceInformation(
	_In_opt_ PSYSTEM_PERFORMANCE_INFORMATION information
)
{
	return NtQuerySystemInformation(
		SystemPerformanceInformation,
		information,
		sizeof(SYSTEM_PERFORMANCE_INFORMATION),
		NULL
	);
}