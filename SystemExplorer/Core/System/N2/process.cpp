#include "pch.h"
#include "process.h"

NTSTATUS SYSX_IMPL_OpenProcess(
	_Out_opt_ PHANDLE process,
	_In_ ACCESS_MASK access,
	_In_ HANDLE id
)
{
	OBJECT_ATTRIBUTES attributes;
	CLIENT_ID client;

	client.UniqueProcess = id;
	client.UniqueThread = NULL;

	InitializeObjectAttributes(&attributes, NULL, 0, NULL, NULL)

	return NtOpenProcess(
		process,
		access,
		&attributes,
		&client
	);
}

NTSTATUS SYSX_IMPL_OpenProcessToken(
	_Out_opt_ PHANDLE token,
	_In_ HANDLE process,
	_In_ ACCESS_MASK access
)
{
	return NtOpenProcessToken(
		process,
		access,
		token
	);
}

NTSTATUS SYSX_IMPL_TerminateProcess(
	_In_ HANDLE ProcessHanlde,
	_In_ NTSTATUS ExitStatus
)
{
	return NtTerminateProcess(
		ProcessHanlde,
		ExitStatus
	);
}