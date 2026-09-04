#include "pch.h"
#include <Core/Kse/KernelDriver.h>
#include "process.h"

using namespace winrt::SystemExplorer::Core;

NTSTATUS SeOpenProcess(
	_Out_opt_ PHANDLE process,
	_In_ ACCESS_MASK access,
	_In_ HANDLE id
)
{
	if (Kernel::KernelDriver::IsConnectionValid())
	{
		Kernel::DriverMessage message;
		Kernel::init_message(&message, KseMsgOpenProcess);

		message.User.OpenProcess.ProcessHandle = process;
		message.User.OpenProcess.DesiredAccess = access;
		message.User.OpenProcess.ClientId->UniqueProcess = id;
		message.User.OpenProcess.ClientId->UniqueThread = NULL;

		if (auto status = Kernel::KernelDriver::SendNewMessage(&message); status > 0)
		{
			return message.User.OpenProcess.Status;
		}
		else
			return status;
	}

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
	_Out_ PUNICODE_STRING FileName
)
{
	if (!FileName)
	{
		return STATUS_INVALID_PARAMETER;
	}

	FileName->Length = 0;
	FileName->MaximumLength = 0;
	FileName->Buffer = nullptr;

	ULONG bufferLength = sizeof(UNICODE_STRING) + (MAX_PATH * sizeof(WCHAR));
	std::vector<std::byte> buffer(bufferLength);
	ULONG returnLength = 0;

	NTSTATUS status = NtQueryInformationProcess(
		ProcessHandle,
		ProcessImageFileNameWin32,
		buffer.data(),
		static_cast<ULONG>(buffer.size()),
		&returnLength
	);

	if (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		buffer.resize(returnLength);
		status = NtQueryInformationProcess(
			ProcessHandle,
			ProcessImageFileNameWin32,
			buffer.data(),
			static_cast<ULONG>(buffer.size()),
			&returnLength
		);
	}

	if (!NT_SUCCESS(status))
	{
		return status;
	}

	auto* queryResult = reinterpret_cast<PUNICODE_STRING>(buffer.data());
	if (!queryResult || queryResult->Length == 0 || !queryResult->Buffer)
	{
		return STATUS_UNSUCCESSFUL;
	}

	USHORT length = queryResult->Length;
	PWSTR newBuffer = reinterpret_cast<PWSTR>(new (std::nothrow) std::byte[length + sizeof(WCHAR)]);
	if (!newBuffer)
	{
		return STATUS_NO_MEMORY;
	}

	std::memcpy(newBuffer, queryResult->Buffer, length);
	newBuffer[length / sizeof(WCHAR)] = UNICODE_NULL;

	FileName->Length = length;
	FileName->MaximumLength = length + sizeof(WCHAR);
	FileName->Buffer = newBuffer;

	return STATUS_SUCCESS;
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

NTSTATUS SeTerminateProcess(
	_In_ HANDLE ProcessHanlde,
	_In_ NTSTATUS ExitStatus
)
{
	if (Kernel::KernelDriver::IsConnectionValid())
	{
		Kernel::DriverMessage message;
		Kernel::init_message(&message, KseMsgTerminateProcess);

		message.User.TerminateProcess.ExitStatus = ExitStatus;
		message.User.TerminateProcess.ProcessHandle = ProcessHanlde;

		if (auto status = Kernel::KernelDriver::SendNewMessage(&message); status > 0)
		{
			return message.User.TerminateProcess.Status;
		}
		else
			return status;
	}

	return NtTerminateProcess(
		ProcessHanlde,
		ExitStatus
	);
}