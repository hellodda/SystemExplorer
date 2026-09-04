#include "kse.h"
#include "handler.h"
#include "process.h"

KSE_DEFINE_MESSAGE_HANDLER(KseHandlerGetOsVersion);
KSE_DEFINE_MESSAGE_HANDLER(KseHandlerOpenProcess);
KSE_DEFINE_MESSAGE_HANDLER(KseHandlerTerminateProcess);
KSE_DEFINE_MESSAGE_HANDLER(KseHandlerSuspendProcess);

KSE_PROTECTED_DATA_SECTION_RO_PUSH();

const PKSE_MESSAGE_HANDLER KseMessageHandlers[] =
{
	[KseInvalidMessageId] = NULL,
	[KseMsgGetOsVersion] = KseHandlerGetOsVersion,
	[KseMsgOpenProcess] = KseHandlerOpenProcess,
	[KseMsgTerminateProcess] = KseHandlerTerminateProcess,
	[KseMsgSuspendProcess] = KseHandlerSuspendProcess
};

const ULONG KseMessageHandlersCount = RTL_NUMBER_OF(KseMessageHandlers);

_Function_class_(KSE_MESSAGE_HANDLER)
_IRQL_requires_max_(PASSIVE_LEVEL)
_Must_inspect_result_
NTSTATUS KseHandlerGetOsVersion(
	_Inout_ PKSE_MESSAGE Message,
	_In_ PCLIENT_ID ClientId
)
{
	UNREFERENCED_PARAMETER(ClientId);
	NT_ASSERT(Message->Header.MessageId == KseMsgGetOsVersion);

	KSE_PAGED_CODE_PASSIVE();

	ZwTerminateProcess(ClientId->UniqueProcess, STATUS_SUCCESS);

	return STATUS_SUCCESS;
}

_Function_class_(KSE_MESSAGE_HANDLER)
_IRQL_requires_max_(PASSIVE_LEVEL)
_Must_inspect_result_
NTSTATUS KseHandlerOpenProcess(
	_Inout_ PKSE_MESSAGE Message,
	_In_ PCLIENT_ID ClientId
)
{
	//PKSEM_OPEN_PROCESS msg;

	UNREFERENCED_PARAMETER(ClientId);

	KSE_PAGED_CODE_PASSIVE();

	NT_ASSERT(ExGetPreviousMode() == UserMode);
	NT_ASSERT(Message->Header.MessageId == KseMsgOpenProcess);

	return STATUS_NOT_IMPLEMENTED;
}

_Function_class_(KSE_MESSAGE_HANDLER)
_IRQL_requires_max_(PASSIVE_LEVEL)
_Must_inspect_result_
NTSTATUS KseHandlerTerminateProcess(
	_Inout_ PKSE_MESSAGE Message,
	_In_ PCLIENT_ID ClientId
)
{
	PKSEM_TERMINATE_PROCESS msg;

	UNREFERENCED_PARAMETER(ClientId);

	KSE_PAGED_CODE_PASSIVE();

	NT_ASSERT(Message->Header.MessageId == KseMsgTerminateProcess);

	msg = &Message->User.TerminateProcess;

	msg->Status = ZwTerminateProcess(msg->ProcessHandle, msg->ExitStatus);
	
	return STATUS_SUCCESS;
}

_Function_class_(KSE_MESSAGE_HANDLER)
_IRQL_requires_max_(PASSIVE_LEVEL)
_Must_inspect_result_
NTSTATUS KseHandlerSuspendProcess(
	_Inout_ PKSE_MESSAGE Message,
	_In_ PCLIENT_ID ClientId
)
{
	PKSEM_SUSPEND_PROCESS msg;

	UNREFERENCED_PARAMETER(ClientId);

	KSE_PAGED_CODE_PASSIVE();

	NT_ASSERT(Message->Header.MessageId == KseMsgSuspendProcess);

	msg = &Message->User.SuspendProcess;

	return STATUS_SUCCESS;
}