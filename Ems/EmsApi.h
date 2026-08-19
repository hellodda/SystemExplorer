//
//
// Explorer Modular System (EMS) V1
//						     By Hellodda
//
#pragma once
#include "EmsMsgDefs.h"

#define EMS_PORT_NAME L"\\RPC Control\\SysExplorerRpcEndpoint"
#define EMS_CURRENT_VERSION 1

typedef USHORT EMS_OBJECT_ID;

typedef struct _EMS_CAPTURE_BUFFER
{
	ULONG Size;
	struct _EMS_CAPTURE_BUFFER* PreviousCaptureBuffer;
	ULONG PointerCount;
	ULONG BufferEnd;
	ULONG_PTR PointerOffsetsArray[ANYSIZE_ARRAY];
} EMS_CAPTURE_BUFFER, * PEMS_CAPTURE_BUFFER;

typedef struct _EMS_API_MESSAGE
{
	struct
	{
		PORT_MESSAGE PortMessage;
		USHORT Size;
		USHORT ApiVersion;
		EMS_API_NUMBER ApiNumber;
		EMS_OBJECT_ID ObjectId;
		LARGE_INTEGER TimeStamp;
	} Header;

	union
	{
		EMS_CAPTURE_BUFFER EmsCaptureBuffer;
		union
		{
			EMS_MODULE_CONNECT ModuleConnect;
			EMS_MODULE_SUSPEND ModuleSuspend;
			EMS_MODULE_SHUTDOWN ModuleShutdown;
			EMS_QUERY_SERVICE QueryService;

			EMS_OPEN_PROCESS OpenProcess;
			EMS_TERMINATE_PROCESS TerminateProcess;
			EMS_DUMP_PROCESS DumpProcess;
			EMS_GET_EXPLORER_SETTING GetSetting;
			EMS_SET_EXPLORER_SETTING SetSetting;

			EMS_CREATE_BUTTON CreateButton;

			// test
			EMS_SEND_DEBUG_REPORT SendDebugReport;
			EMS_SEND_ANY_DATA SendAnyData;
			EMS_HEALTH_CHECK HealthCheck;
		} Client;

		union
		{
			EMS_MODULE_SUSPEND ModuleSuspend;
			EMS_MODULE_SHUTDOWN ModuleShutdown;
			EMS_HEALTH_CHECK HealthCheck;
		} Host;
	};
} EMS_API_MESSAGE, * PEMS_API_MESSAGE;

#define EMS_MESSAGE_SIZE sizeof(EMS_API_MESSAGE)

VOID EmsQuerySystemTime(
	_Out_ PLARGE_INTEGER SystemTime
);

VOID EmsInitializeMessage(
	_Out_writes_bytes_(EMS_MESSAGE_SIZE) PEMS_API_MESSAGE Message,
	_In_ EMS_API_NUMBER ApiNumber
);

_Must_inspect_result_
NTSTATUS EmsValidateMessage(
	_In_ PEMS_API_MESSAGE Message
);

NTSTATUS EmsConnectToHost(
	_Out_ PHANDLE PortHandle,
	_In_ PEMS_MODULE_CONNECT ModuleConnect
);

NTSTATUS EmsSendMessage(
	_In_ HANDLE PortHandle,
	_In_ PEMS_API_MESSAGE Message
);

