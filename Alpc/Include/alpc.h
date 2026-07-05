#pragma once
#include "../framework.h"


#define ALPC_STATIC

#if defined(ALPC_STATIC)

#define ALPC_API 
#elif defined(ALPC_DLL_EXPORTS)

#define ALPC_API __declspec(dllexport)
#else

#define ALPC_API __declspec(dllimport)
#endif

#define DATA_BUFFER_SIZE 0x480  
#define MAX_MSG_LEN      0x600

#define ALPC_API_PORT_NAME L"\\RPC Control\\SysExplorerRpcEndpoint"

#define ALPC_API_VERSION_IGNORE -1
#define ALPC_API_VERSION_CURRENT 1

#define ALPC_CMD_NONE 0
#define ALPC_CMD_PLUGIN_INIT 1
#define ALPC_CMD_PLUGIN_SUSPEND 2
#define ALPC_CMD_ECHO 3

#define ALPC_CMD_OPEN_PROCESS 100
#define ALPC_CMD_TERMINATE_PROCESS 101

#define ALPC_CMD_CREATE_BUTTON 200
#define ALPC_CMD_DELETE_BUTTON 201
#define ALPC_CMD_LOAD_XAML 202

#define ALPC_CMD_GET_PROCESSES_SERVICE 300
#define ALPC_CMD_GET_XAML_SERVICE 301

//#define CMD_IS_PROCESSES_SERVICE_SUPPORTED
//#define CMD_IS_XAML_SERVICE_SUPPORTED


typedef struct _ALPC_REQUEST_MESSAGE
{
    PORT_MESSAGE Header;
    DWORD Request;
    DWORD Version;
    BYTE Data[DATA_BUFFER_SIZE];
} ALPC_REQUEST_MESSAGE, * PALPC_REQUEST_MESSAGE;

typedef struct _ALPC_RESPONSE_MESSAGE
{
    PORT_MESSAGE Header;
    DWORD Response;
    NTSTATUS Status;
    BYTE Data[DATA_BUFFER_SIZE];
} ALPC_RESPONSE_MESSAGE, * PALPC_RESPONSE_MESSAGE;

typedef struct _PLUGIN_INFORMATION
{
    DWORD Reserved;
    DWORD Version;
    GUID Guid;
    WCHAR Author[32];
    WCHAR Name[32];
} PLUGIN_INFORMATION, * PPLUGIN_INFORMATION;

typedef enum _BUTTON_LOCATION
{
    blMainWindowMenuFlyout,
    blProcessesListMenuFlyout,
    blServicesListMenuFlyout
} BUTTON_LOCATION, * PBUTTON_LOCATION;

typedef struct _BUTTON_INFORMATION
{
    DWORD CommandId;
    BUTTON_LOCATION Location;
    WCHAR Text[64];
    WCHAR TooltipSuggestionText[64];
} BUTTON_INFORMATION, * PBUTTON_INFORMATION;

#ifdef __cplusplus
extern "C" {
#endif

    ALPC_API NTSTATUS AlpcPluginStartup(
        _Out_ PHANDLE connectionHandle,
        _In_ PPLUGIN_INFORMATION information
    );

    ALPC_API ALPC_RESPONSE_MESSAGE AlpcSendCommand(
        _In_ HANDLE connectionHandle,
        _In_ DWORD requestId,
        _In_opt_ PVOID payload,
        _In_ SIZE_T payloadSize
    );

    ALPC_API NTSTATUS AlpcCreateButton(
        _In_ HANDLE connectionHandle,
        _In_ PBUTTON_INFORMATION information
    );

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include "alpcplus.h"
#endif