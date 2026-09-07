#pragma once
#include "emsbase.h"
#include "emsmsgdefs.h"

typedef enum _EMS_MESSAGE_ID
{
	InvalidEmsMessageId,

	EmsMessageIdRegisterModule,
	EmsMessageIdContent,

	MaxEmsMessageId
} EMS_MESSAGE_ID;

typedef enum _EMS_API_MESSAGE_TRANSPORT_TYPE
{
	UnknownEmsApiMessageTransport,

	EmsApiMessagePipeTransport,
	EmsApiMessageAlpcTransport,

	MaxEmsApiMessageTransport
} EMS_API_MESSAGE_TRANSPORT_TYPE;

typedef struct _EMS_API_MESSAGE
{
	struct
	{

#ifdef SYSX_TRANSPORT_TYPE_ALPC
		PORT_MESSAGE PortMessage;
#endif 

		SHORT ApiVersion;
		EMS_API_MESSAGE_TRANSPORT_TYPE TransportType;
		EMS_MESSAGE_ID MessageId;
	} Header;

	struct
	{
		EMSM_REGISTER_MODULE RegisterModule;
	} Module;

} EMS_API_MESSAGE, *PEMS_API_MESSAGE;

#ifdef __cplusplus
extern "C" {
#endif

SYSX_EMS_API
_Must_inspect_result_
EMS_STATUS EmsValidateMessage(
	_In_ PEMS_API_MESSAGE Message
);

SYSX_EMS_API
VOID EmsInitMessage(
	_Out_writes_bytes_(sizeof(EMS_API_MESSAGE)) PEMS_API_MESSAGE Message,
	_In_ EMS_MESSAGE_ID MessageId
);

SYSX_EMS_API
_Must_inspect_result_
EMS_STATUS EmsSendMessage(
	_In_ HANDLE ConnectionHandle,
	_Inout_ PEMS_API_MESSAGE Message
);

SYSX_EMS_API
_Must_inspect_result_
EMS_STATUS EmsConnectToHost(
	_Inout_ PHANDLE ConnectionHandle,
	_In_ PEMSM_REGISTER_MODULE ModuleRegistrationMessage
);

#ifdef __cplusplus
}
#endif