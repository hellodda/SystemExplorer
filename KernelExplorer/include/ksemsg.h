#pragma once
#include "kse.h"
#include "ksemsgdefs.h"

#pragma warning(push)
#pragma warning(disable : 4201)

typedef struct _KSE_MESSAGE
{
	struct 
	{
		KSE_MESSAGE_ID MessageId;
		ULONG Size;
	} Header;

	union
	{
		KSEM_GET_OS_VERSION GetOsVersion;
		KSEM_OPEN_PROCESS OpenProcess;
	} User;
} KSE_MESSAGE, *PKSE_MESSAGE;


typedef
_Function_class_(KSE_MESSAGE_HANDLER)
_IRQL_requires_max_(PASSIVE_LEVEL)
_Must_inspect_result_
NTSTATUS
KSE_MESSAGE_HANDLER(
	_Inout_ PKSE_MESSAGE Message
);
typedef KSE_MESSAGE_HANDLER* PKSE_MESSAGE_HANDLER;

#define KSE_DEFINE_MESSAGE_HANDLER(name)             \
_Function_class_(KSE_MESSAGE_HANDLER)                \
_IRQL_requires_max_(PASSIVE_LEVEL)                   \
_Must_inspect_result_                                \
NTSTATUS name(                                       \
    _Inout_ PKSE_MESSAGE Message                     \
)																		

