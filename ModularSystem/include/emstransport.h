#pragma once
#include "emsbase.h"
#include "emsmsg.h"

EMS_STATUS EmspSendMessageWithAlpc(
	_In_ HANDLE ConnectionHandle,
	_Inout_ PEMS_API_MESSAGE Message
);

EMS_STATUS EmspSendMessageWithPipes(
	_In_ HANDLE ConnectionHandle,
	_Inout_ PEMS_API_MESSAGE Message
);

EMS_STATUS EmspConnectWithAlpc(
	_Inout_ PHANDLE ConnectionHandle
);

EMS_STATUS EmspConnectWithPipes(
	_Inout_ PHANDLE ConnectionHandle
);