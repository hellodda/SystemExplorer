#pragma once

typedef enum _KSE_MESSAGE_ID
{
	KseInvalidMessageId,

	KseMsgGetOsVersion,
	KseMsgOpenProcess,

	KseMaxMessageId
} KSE_MESSAGE_ID;

typedef struct _KSEM_GET_OS_VERSION
{
	NTSTATUS Status;
	ULONG Major;
	ULONG Minor;
	ULONG Build;
} KSEM_GET_OS_VERSION, *PKSE_GET_OS_VERSION;

typedef struct _KSEM_OPEN_PROCESS
{
	OUT NTSTATUS Status;
	INOUT PHANDLE ProcessHandle;
	IN ACCESS_MASK DesiredAccess;
	IN PCLIENT_ID ClientId;
} KSEM_OPEN_PROCESS, *PKSEM_OPEN_PROCESS;