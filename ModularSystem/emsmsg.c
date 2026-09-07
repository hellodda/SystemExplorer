#include "include/emsmsg.h"
#include "include/emstransport.h"

_Must_inspect_result_
EMS_STATUS EmsValidateMessage(
	_In_ PEMS_API_MESSAGE Message
)
{
	if (!Message)
		return E_INVALIDARG;

	return S_OK;
}

VOID EmsInitMessage(
	_Out_writes_bytes_(sizeof(EMS_API_MESSAGE)) PEMS_API_MESSAGE Message,
	_In_ EMS_MESSAGE_ID MessageId
)
{
	RtlZeroMemory(Message, sizeof(EMS_API_MESSAGE));
	Message->Header.ApiVersion = 0;
#ifdef SYSX_TRANSPORT_TYPE_ALPC 
	Message->Header.TransportType = EmsApiMessageAlpcTransport;
#else
	Message->Header.TransportType = EmsApiMessagePipeTransport;
#endif
	Message->Header.MessageId = MessageId;
}

_Must_inspect_result_
EMS_STATUS EmsSendMessage(
	_In_ HANDLE ConnectionHandle,
	_Inout_ PEMS_API_MESSAGE Message
)
{

#ifdef SYSX_TRANSPORT_TYPE_ALPC

	return EmspSendMessageWithAlpc(ConnectionHandle, Message);

#else

	return EmspSendMessageWithPipes(ConnectionHandle, Message);

#endif // SYSX_TRANSPORT_TYPE_ALPC

}

_Must_inspect_result_
EMS_STATUS EmsConnectToHost(
	_Inout_ PHANDLE ConnectionHandle,
	_In_ PEMSM_REGISTER_MODULE ModuleRegistrationMessage
)
{
	if (!ModuleRegistrationMessage)
		return E_INVALIDARG;

	EMS_STATUS status;
	
#ifdef SYSX_TRANSPORT_TYPE_ALPC

	status = EmspConnectWithAlpc(ConnectionHandle);

#else

	status = EmspConnectWithPipes(ConnectionHandle);

#endif // SYSX_TRANSPORT_TYPE_ALPC

	if (SUCCEEDED(status))
	{
		EMS_API_MESSAGE connectionMessage;

		EmsInitMessage(&connectionMessage, EmsMessageIdRegisterModule);
		connectionMessage.Module.RegisterModule = *ModuleRegistrationMessage;

		status = EmsSendMessage(ConnectionHandle, &connectionMessage);

		return status;
	}
	return status;
}