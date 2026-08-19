#include "pch.h"
#include "EmsServiceProvider.h"
#include "EmsApi.h"

STDMETHODIMP CEmsServiceProvider::Host(
    _In_ HANDLE hHost
)
{
    if (hHost == INVALID_HANDLE_VALUE || !hHost)
        return E_POINTER;

    host_ = hHost;

    return S_OK;
}

BOOL CEmsServiceProvider::IsServiceSupported(EMS_SERVICE_TYPE service)
{
    EMS_API_MESSAGE message;
    
    EmsInitializeMessage(&message, EmsApiQueryService);

    message.Client.QueryService.ServiceType = service;

    auto status = EmsSendMessage(host_, &message);
    
    if (NT_ERROR(status))
        return FALSE;

    if (NT_ERROR(message.Client.QueryService.Status))
        return FALSE;

    return TRUE;
}

