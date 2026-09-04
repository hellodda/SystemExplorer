#pragma once
#include "ksemsg.h"


typedef
_Function_class_(KSE_MESSAGE_HANDLER)
_IRQL_requires_max_(PASSIVE_LEVEL)
_Must_inspect_result_
NTSTATUS
KSE_MESSAGE_HANDLER(
	_Inout_ PKSE_MESSAGE Message,
	_In_ PCLIENT_ID ClientId
);
typedef KSE_MESSAGE_HANDLER* PKSE_MESSAGE_HANDLER;

#define KSE_DEFINE_MESSAGE_HANDLER(name)             \
_Function_class_(KSE_MESSAGE_HANDLER)                \
_IRQL_requires_max_(PASSIVE_LEVEL)                   \
_Must_inspect_result_                                \
NTSTATUS name(                                       \
    _Inout_ PKSE_MESSAGE Message,                    \
	_In_ PCLIENT_ID ClientId					     \
)																		


extern const PKSE_MESSAGE_HANDLER KseMessageHandlers[];
extern const ULONG KseMessageHandlersCount;
