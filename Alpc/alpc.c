#include "framework.h"
#include "alpc.h"

NTSTATUS AlpcPluginStartup(
    _Out_ PHANDLE connectionHandle,
    _In_ PPLUGIN_INFORMATION information
)
{
    UNICODE_STRING usPort;
    ALPC_PORT_ATTRIBUTES portAttr;
    ALPC_REQUEST_MESSAGE connectionMessage;
    ULONG bufferLength;

    if (connectionHandle == NULL || information == NULL)
    {
        return STATUS_INVALID_PARAMETER;
    }

    RtlInitUnicodeString(&usPort, ALPC_API_PORT_NAME);

    RtlSecureZeroMemory(&portAttr, sizeof(portAttr));
    portAttr.MaxMessageLength = sizeof(ALPC_REQUEST_MESSAGE);

    RtlSecureZeroMemory(&connectionMessage, sizeof(connectionMessage));
    connectionMessage.Request = ALPC_CMD_PLUGIN_INIT;
    connectionMessage.Version = ALPC_API_VERSION_IGNORE;
    connectionMessage.Header.u1.s1.DataLength = (CSHORT)(sizeof(ALPC_REQUEST_MESSAGE) - sizeof(PORT_MESSAGE));
    connectionMessage.Header.u1.s1.TotalLength = (CSHORT)sizeof(ALPC_REQUEST_MESSAGE);
    memcpy(connectionMessage.Data, information, sizeof(PLUGIN_INFORMATION));

    bufferLength = sizeof(connectionMessage);

    return NtAlpcConnectPort(
        connectionHandle,
        &usPort,
        NULL,
        &portAttr,
        0,
        NULL,
        (PPORT_MESSAGE)&connectionMessage,
        &bufferLength,
        NULL,
        NULL,
        NULL
    );
}

ALPC_RESPONSE_MESSAGE AlpcSendCommand(
    _In_ HANDLE connectionHandle,
    _In_ DWORD requestId,
    _In_opt_ PVOID payload,
    _In_ SIZE_T payloadSize
)
{
    if (connectionHandle == INVALID_HANDLE_VALUE || connectionHandle == NULL)
    {

    }

    ALPC_REQUEST_MESSAGE requestMessage;
    RtlSecureZeroMemory(&requestMessage, sizeof(requestMessage));
    requestMessage.Version = ALPC_API_VERSION_CURRENT;
    requestMessage.Request = requestId;

    if (payload != NULL && payloadSize > 0)
    {
        memcpy(requestMessage.Data, payload, payloadSize);
    }

    requestMessage.Header.u1.s1.DataLength = sizeof(ALPC_REQUEST_MESSAGE) - sizeof(PORT_MESSAGE);
    requestMessage.Header.u1.s1.TotalLength = sizeof(ALPC_REQUEST_MESSAGE);

    ALPC_RESPONSE_MESSAGE responseMessage;
    SIZE_T responseLength = sizeof(ALPC_RESPONSE_MESSAGE);
    RtlSecureZeroMemory(&responseMessage, responseLength);

    NTSTATUS status = NtAlpcSendWaitReceivePort(
        connectionHandle,
        NULL,
        (PPORT_MESSAGE)&requestMessage,
        NULL,
        (PPORT_MESSAGE)&responseMessage,
        &responseLength,
        NULL,
        NULL
    );
    responseMessage.Status = status;

    return responseMessage;
}

NTSTATUS AlpcCreateButton(
    HANDLE connectionHandle,
    PBUTTON_INFORMATION information
)
{
    return STATUS_NOT_IMPLEMENTED;
}

