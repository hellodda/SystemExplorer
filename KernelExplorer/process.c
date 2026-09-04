#include "process.h"

_IRQL_requires_max_(PASSIVE_LEVEL)
_Must_inspect_result_
NTSTATUS KseOpenProcess(
	_Out_ PHANDLE ProcessHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_ PCLIENT_ID ClientId,
	_In_ KPROCESSOR_MODE AccessMode
) 
{
	UNREFERENCED_PARAMETER(ProcessHandle);
	UNREFERENCED_PARAMETER(DesiredAccess);
	UNREFERENCED_PARAMETER(ClientId);
	UNREFERENCED_PARAMETER(AccessMode);

	return STATUS_NOT_IMPLEMENTED;
}