#pragma once
#include "kse.h"
#include "ksemsgdefs.h"

#pragma warning(push)
#pragma warning(disable : 4201)

typedef struct _KSE_MESSAGE
{
	struct 
	{
		USHORT Version;
		USHORT Size;
		KSE_MESSAGE_ID MessageId;
		LARGE_INTEGER TimeStamp;
	} Header;

	union
	{
		union
		{
			KSEM_GET_OS_VERSION GetOsVersion;
			KSEM_OPEN_PROCESS OpenProcess;
			KSEM_TERMINATE_PROCESS TerminateProcess;
			KSEM_SUSPEND_PROCESS SuspendProcess;
		} User;

		union
		{
			int a;
		} Kernel;

		union
		{
			int a;
		} Reply;
	};
} KSE_MESSAGE, *PKSE_MESSAGE;

typedef CONST PKSE_MESSAGE PCKSE_MESSAGE;

VOID KseQuerySystemTime(
	_Out_ PLARGE_INTEGER SystemTime
);

VOID KseInitializeMessage(
	_Out_writes_bytes_(sizeof(KSE_MESSAGE)) PKSE_MESSAGE Message,
	_In_ KSE_MESSAGE_ID MessageId
);

_Must_inspect_result_
NTSTATUS KseValidateMessage(
	_In_ PCKSE_MESSAGE Message
);
