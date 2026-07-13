#pragma once
#include <windows.h>
#include <phnt_windows.h>
#include <phnt.h>

typedef enum _EMS_API_NUMBER
{
	EmsInvalidApiNumber,

	//
	// Module --> SystemExplorer
	//

	EmsApiModuleConnect,
	EmsApiOpenProcess,
	EmsApiTerminateProcess,
	EmsApiDumpProcess,
	EmsApiDumpKernel,
	EmsApiCreateButton,
	EmsApiSendAnyData, // test
	EmsApiSendDebugReport,
	EmsApiGetSetting,
	EmsApiSetSetting,

	//
	// SystemExplorer --> Module
	// SystemExplorer <-- Module
	
	EmsApiHealthCheck,
	EmsApiModuleShutdown,
	EmsApiModuleSuspend,

	EmsMaxApiNumber
} EMS_API_NUMBER;

typedef struct _EMS_MODULE_CONNECT
{
	OUT NTSTATUS Status;
	IN GUID Guid;
	IN WCHAR Name[32];
	IN WCHAR Author[32];
	IN WCHAR Description[101];
} EMS_MODULE_CONNECT, *PEMS_MODULE_CONNECT;

typedef enum _EMS_MODULE_SHUTDOWN_REASON
{
	EmsInvalidReason,

	// 
	// SystemExplorer --> Module
	// SystemExplorer <-- Module
	//

	EmsReasonCriticalError,
	EmsReasonComplited,

	EmsMaxReason
} EMS_MODULE_SHUTDOWN_REASON; 

typedef struct _EMS_MODULE_SHUTDOWN
{
	OUT NTSTATUS Status;
	IN EMS_MODULE_SHUTDOWN_REASON Reason;
} EMS_MODULE_SHUTDOWN, *PEMS_MODULE_SHUTDOWN;

typedef enum _EMS_MODULE_SUSPEND_TYPE
{
	EmsSuspendTypeUser,
	EmsSuspendTypeHost
} EMS_MODULE_SUSPEND_TYPE;

typedef struct _EMS_MODULE_SUSPEND
{
	OUT union { NTSTATUS Status; };
	IN EMS_MODULE_SUSPEND_TYPE Type;
} EMS_MODULE_SUSPEND, *PEMS_MODULE_SUSPEND;

typedef struct _EMS_OPEN_PROCESS
{
	OUT NTSTATUS Status;
	OUT HANDLE ProcessHandle;
	IN ACCESS_MASK DesiredAccess;
	IN CLIENT_ID ClientId;
} EMS_OPEN_PROCESS, *PEMS_OPEN_PROCESS;

typedef struct _EMS_TERMINATE_PROCESS
{
	OUT NTSTATUS Status;
	IN HANDLE ProcessHandle;
	IN NTSTATUS ExitStatus;
} EMS_TERMINATE_PROCESS, *PEMS_TERMINATE_PROCESS;

typedef struct _EMS_DUMP_PROCESS
{
	OUT NTSTATUS Status;
	IN HANDLE ProcessHandle;
} EMS_DUMP_PROCESS, *PEMS_DUMP_PROCESS;

typedef enum _EMS_BUTTON_CREATION_LOCATION
{

} EMS_BUTTON_CREATION_LOCATION;

typedef struct _EMS_CREATE_BUTTON
{
	OUT NTSTATUS Status;
	IN EMS_BUTTON_CREATION_LOCATION Location;
	
	/*IN union 
	{
		
	} Content;*/
} EMS_CREATE_BUTTON, *PEMS_CREATE_BUTTON;

typedef enum _EMS_EXPLORER_SETTING_TYPE
{
	EmsGetExplorerGeneralSettings,
	EmsGetExplorerAdvancedSettings
} EMS_EXPLORER_SETTING_TYPE;

typedef struct _EMS_EXPLORER_SETTINGS
{
	union 
	{
		struct
		{
			UINT Language;
			UINT RealTimeUpdateSpeed;
			UINT DefaultStartupPage;

			struct
			{
				BOOLEAN AlwaysOnTop;
				BOOLEAN MinimizeOnUse;
				BOOLEAN HideWhenMinimized;
			} WindowManagement;

			struct
			{
				BOOLEAN AbortIfInsufficentMemory;
				BOOLEAN CaptureHypervisorPages;
				BOOLEAN IncludeNonessentialPages;
				BOOLEAN CaptureUserPages;
			} KernelDumpParameters;

			BOOLEAN ResetSettings;
		} GeneralSettings;

		struct
		{
			UINT ApplicationTheme;
			UINT ApplicationBackgroundColor;
			UINT ApplicationBackgroundMaterial;
			
			struct
			{
				UINT32 Path;
				UINT32 Opacity;
				UINT32 Mode;
				UINT32 Horizontal;
				UINT32 Vertical;
			} ApplicationBackgroundImage;
		} AppearanceSettings;

		struct
		{
			BOOLEAN AdvancedAiActions;
			BOOLEAN IsDefaultTaskManager;
			BOOLEAN UseDriverAsDataSource;
		} AdvancedSettings;
	};
} EMS_EXPLORER_SETTINGS, *PEMS_EXPLORER_SETTINGS;

typedef struct _EMS_GET_EXPLORER_SETTING
{
	OUT NTSTATUS Status;
	IN EMS_EXPLORER_SETTING_TYPE Type;
	OUT EMS_EXPLORER_SETTINGS Value;
} EMS_GET_EXPLORER_SETTING, *PEMS_GET_EXPLORER_SETTING;

typedef struct _EMS_SET_EXPLORER_SETTING
{
	OUT NTSTATUS Status;
	IN EMS_EXPLORER_SETTING_TYPE Type;
} EMS_SET_EXPLORER_SETTING, *PEMS_SET_EXPLORER_SETTING;

typedef struct _EMS_SEND_ANY_DATA
{
	NTSTATUS Status;
	BYTE Data[ANYSIZE_ARRAY];
} EMS_SEND_ANY_DATA, *PEMS_SEND_ANY_DATA;

typedef struct _EMS_SEND_DEBUG_REPORT
{
	OUT NTSTATUS Status;
	IN union
	{
		HRESULT Hresult;
		NTSTATUS NtStatus;
		DWORD Win32;
	} Warning;
	IN WCHAR* Message;
} EMS_SEND_DEBUG_REPORT, *PEMS_SEND_DEBUG_REPORT;


typedef struct _EMS_HEALTH_CHECK
{
	NTSTATUS Status;
} EMS_HEALTH_CHECK, *PEMS_HEALTH_CHECK;

typedef struct _EMS_EXECUTE_WQL
{
	int a;
} EMS_EXECUTE_WQL, *PEMS_EXECUTE_WQL;