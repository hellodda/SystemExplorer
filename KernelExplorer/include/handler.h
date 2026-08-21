#pragma once
#include "ksemsg.h"

typedef struct _KSE_DISPATCH_ENTRY
{
	KSE_MESSAGE_ID MessageId;
	PKSE_MESSAGE_HANDLER MessageHandler;
} KSE_DISPATCH_ENTRY, * PKSE_DISPATCH_ENTRY;

extern const KSE_DISPATCH_ENTRY KseMessageHandlers[];
extern const ULONG KseMessageHandlersCount;