#include "kse.h"
#include "handler.h"
#include "process.h"

KSE_DEFINE_MESSAGE_HANDLER(KseHandlerGetOsVersion);
KSE_DEFINE_MESSAGE_HANDLER(KseHandlerOpenProcess);

KSE_PROTECTED_DATA_SECTION_RO_PUSH();

const KSE_DISPATCH_ENTRY KseMessageHandlers[] =
{
	{ KseInvalidMessageId, NULL },
	{ KseMsgGetOsVersion, KseHandlerGetOsVersion },
	{ KseMsgOpenProcess, KseHandlerOpenProcess }
};

const ULONG KseMessageHandlersCount = RTL_NUMBER_OF(KseMessageHandlers);

_Function_class_(KSE_MESSAGE_HANDLER)
_IRQL_requires_max_(PASSIVE_LEVEL)
_Must_inspect_result_
NTSTATUS KseHandlerGetOsVersion(
	_Inout_ PKSE_MESSAGE Message
)
{
	KSE_PAGED_CODE_PASSIVE();

	Message->User.GetOsVersion.Major = 100;
	Message->User.GetOsVersion.Minor = 101;
	Message->User.GetOsVersion.Build = 102;
	Message->User.GetOsVersion.Status = STATUS_AAD_CLOUDAP_E_ASSERTION_MALFORMED;

	return STATUS_SUCCESS;
}

_Function_class_(KSE_MESSAGE_HANDLER)
_IRQL_requires_max_(PASSIVE_LEVEL)
_Must_inspect_result_
NTSTATUS KseHandlerOpenProcess(
	_Inout_ PKSE_MESSAGE Message
)
{
	PKSEM_OPEN_PROCESS msg;

	KSE_PAGED_CODE_PASSIVE();

	NT_ASSERT(ExGetPreviousMode() == UserMode);
	NT_ASSERT(Message->Header.MessageId == KseMsgOpenProcess);

	msg = &Message->User.OpenProcess;

	msg->Status = KseOpenProcess(msg->ProcessHandle,
								 msg->DesiredAccess,
								 msg->ClientId,
								 UserMode);

	return STATUS_SUCCESS;
}