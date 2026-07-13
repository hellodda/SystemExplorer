#include <kse.h>
#include <comms.h>
#include <ksemsgdefs.h>

KSE_DEFINE_MESSAGE_HANDLER(KseCommsOpenProcess);

_Function_class_(KSE_MESSAGE_HANDLER)
_IRQL_requires_max_(PASSIVE_LEVEL)
_Must_inspect_result_
NTSTATUS KSIAPI KseCommsOpenProcess(
	_In_ PKSE_CLIENT Client,
	_Inout_ PKSE_MESSAGE Message
)
{
	KSEM_OPEN_PROCESS message;

	KSE_PAGED_CODE_PASSIVE();
	NT_ASSERT(ExGetPreviousMode() == UserMode);
	//NT_ASSERT()

	UNREFERENCED_PARAMETER(Client);


	return STATUS_SUCCESS;
}