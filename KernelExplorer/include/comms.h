#pragma once

#include "ksemsg.h"

typedef struct _KSE_CLIENT_EXPLORER_STATE
{
	KSE_MESSAGE_TIMEOUTS MessageTimeouts;
};

typedef struct _KSE_CLIENT
{

} KSE_CLIENT, *PKSE_CLIENT;

typedef
_Function_class_(KSE_MESSAGE_HANDLER)
_IRQL_requires_max_(PASSIVE_LEVEL)
_Must_inspect_result_
NTSTATUS
KSE_MESSAGE_HANDLER(
	_In_ PKSE_CLIENT Client,
	_Inout_ PKSE_MESSAGE Message
);
typedef KSE_MESSAGE_HANDLER* PKSE_MESSAGE_HANDLER;

#define KSE_DEFINE_MESSAGE_HANDLER(name)									\
_Function_class_(KSE_MESSAGE_HANDLER)										\
_IRQL_requires_max_(PASSIVE_LEVEL)                                          \
_Must_inspect_result_                                                       \
NTSTATUS name(																\
	_In_ PKSE_CLIENT Client,												\
	_Inout_ PKSE_MESSAGE Message                                            \
)