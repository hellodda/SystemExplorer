#include "ems.h"

VOID EmsQuerySystemTime(
	_Out_ PLARGE_INTEGER SystemTime
)
{
	while (TRUE)
	{
		SystemTime->HighPart = USER_SHARED_DATA->SystemTime.High1Time;
		SystemTime->LowPart = USER_SHARED_DATA->SystemTime.LowPart;

		if (SystemTime->HighPart == USER_SHARED_DATA->SystemTime.High2Time)
			break;

		YieldProcessor();
	}
}

VOID EmsInitializeMessage(
	_Out_writes_bytes_(EMS_MESSAGE_SIZE) PEMS_API_MESSAGE Message,
	_In_ EMS_API_NUMBER ApiNumber
)
{
	RtlZeroMemory(Message, EMS_MESSAGE_SIZE);
	Message->Header.ApiVersion = EMS_CURRENT_VERSION;
	Message->Header.Size = EMS_MESSAGE_SIZE;
	Message->Header.PortMessage.u1.s1.DataLength = EMS_MESSAGE_SIZE - sizeof(PORT_MESSAGE);
	Message->Header.PortMessage.u1.s1.TotalLength = EMS_MESSAGE_SIZE;
	Message->Header.ApiNumber = ApiNumber;
	EpmQuerySystemTime(&Message->Header.TimeStamp);
}

_Must_inspect_result_
NTSTATUS EmsValidateMessage(
	_In_ PEMS_API_MESSAGE Message
)
{
	if (Message->Header.ApiVersion != EMS_CURRENT_VERSION)
		return STATUS_REVISION_MISMATCH;

	if (Message->Header.Size < EMS_MESSAGE_SIZE)
		return STATUS_INVALID_MESSAGE;

	return STATUS_SUCCESS;
}

NTSTATUS EmsConnectToHost(
	_Out_ PHANDLE PortHandle,
	_In_ PEMS_MODULE_CONNECT ModuleConnect
)
{
	if (ModuleConnect == NULL)
		return STATUS_INVALID_PARAMETER;

	NTSTATUS status;
	UNICODE_STRING portName;
	ALPC_PORT_ATTRIBUTES portAttributes;
	EMS_API_MESSAGE message;
	ULONG bufferLength;

	RtlInitUnicodeString(&portName, EMS_PORT_NAME);
	RtlSecureZeroMemory(&portAttributes, sizeof(portAttributes));
	portAttributes.MaxMessageLength = EMS_MESSAGE_SIZE;

	EmsInitializeMessage(&message, EmsApiModuleConnect);
	message.Client.ModuleConnect = *ModuleConnect;
	bufferLength = sizeof(message);

	status = NtAlpcConnectPort(
		PortHandle,
		&portName,
		NULL,
		&portAttributes,
		0,
		NULL,
		(PPORT_MESSAGE)&message,
		&bufferLength,
		NULL,
		NULL,
		NULL
	);

	if (!NT_SUCCESS(status))
		return status;

	return message.Client.ModuleConnect.Status;
}

NTSTATUS EmsSendMessage(
	_In_ HANDLE PortHandle,
	_In_ PEMS_API_MESSAGE Message
)
{
	if (PortHandle == INVALID_HANDLE_VALUE || PortHandle == NULL)
		return STATUS_INVALID_HANDLE;

	if (!NT_SUCCESS(EmsValidateMessage(Message)))
		return STATUS_INVALID_MESSAGE;

	SIZE_T bufferLength = Message->Header.PortMessage.u1.s1.TotalLength;

	return NtAlpcSendWaitReceivePort(
		PortHandle,
		0,
		(PPORT_MESSAGE)Message,
		NULL,
		(PPORT_MESSAGE)Message,
		&bufferLength,
		NULL,
		NULL
	);
}