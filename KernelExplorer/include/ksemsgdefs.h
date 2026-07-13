#pragma once

typedef struct _KSEM_OPEN_PROCESS
{
	NTSTATUS Status;
	PHANDLE ProcessHandle;
	ACCESS_MASK DesiredAccess;
	PCLIENT_ID ClientId;
} KSEM_OPEN_PROCESS, *PKSEM_OPEN_PROCESS;