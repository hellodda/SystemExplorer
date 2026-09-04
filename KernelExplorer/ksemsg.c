#include "ksemsg.h"

VOID KseQuerySystemTime(
	_Out_ PLARGE_INTEGER SystemTime
)
{
#ifdef _KERNEL_MODE
	KeQuerySystemTime(SystemTime);
#else
	while (TRUE)
	{
		SystemTime->HighPart = USER_SHARED_DATA->SystemTime.High1Time;
		SystemTime->LowPart = USER_SHARED_DATA->SystemTime.LowPart;

		if (SystemTime->HighPart == USER_SHARED_DATA->SystemTime.High2Time)
			break;

		YieldProcessor();
	}
#endif
}

VOID KseInitializeMessage(
	_Out_writes_bytes_(sizeof(KSE_MESSAGE)) PKSE_MESSAGE Message,
	_In_ KSE_MESSAGE_ID MessageId
)
{
	RtlZeroMemory(Message, sizeof(KSE_MESSAGE));
	Message->Header.Version = 1;
	Message->Header.Size = sizeof(KSE_MESSAGE);
	Message->Header.MessageId = MessageId;
	KseQuerySystemTime(&Message->Header.TimeStamp);
}

_Must_inspect_result_
NTSTATUS KseValidateMessage(
	_In_ PCKSE_MESSAGE Message
)
{
	if (Message == NULL)
	{
		return STATUS_INVALID_MESSAGE;
	}

	if ((Message->Header.Size < sizeof(KSE_MESSAGE)) ||
		(Message->Header.Size > sizeof(KSE_MESSAGE)))
	{
		return STATUS_INVALID_MESSAGE;
	}

	if (Message->Header.MessageId != KseUnhandledMessageId)
	{
		if ((Message->Header.MessageId <= KseInvalidMessageId) ||
			(Message->Header.MessageId >= KseMaxMessageId))
		{
			return STATUS_INVALID_MESSAGE;
		}
	}
	return STATUS_SUCCESS;
}
