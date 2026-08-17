#pragma once

NTSTATUS SYSX_IMPL_GetSystemBasicInformation(
	_In_opt_ PSYSTEM_BASIC_INFORMATION information
);

NTSTATUS SYSX_IMPL_GetSystemPerformanceInformation(
	_In_opt_ PSYSTEM_PERFORMANCE_INFORMATION information 
);
