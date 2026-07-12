#include <ems.h>

_Success_(return == EXIT_SUCCESS)
INT wmain()
{
	EMS_MODULE_CONNECT moduleConnect;
	HANDLE portHandle;

	EmsConnectToHost(&portHandle, &moduleConnect);

	EMS_API_MESSAGE shutdownMessage;
	EmsInitializeMessage(&shutdownMessage, EmsApiModuleShutdown);
	shutdownMessage.Client.ModuleShutdown.Reason = EmsReasonComplited;

	EMS_API_MESSAGE getDriverSettingMessage;
	EmsInitializeMessage(&getDriverSettingMessage, EmsApiGetSetting);
	getDriverSettingMessage.Client.GetSetting.Type = EmsGetExplorerAdvancedSettings;

	EmsSendMessage(&portHandle, &getDriverSettingMessage);
	
	if (getDriverSettingMessage.Client.GetSetting.Value.AdvancedSettings.UseDriverAsDataSource == TRUE)
	{

	}
	EMS_EXPLORER_SETTINGS settings;


	EmsSendMessage(portHandle, &shutdownMessage);

	auto handle = OpenProcess(NULL, NULL, NULL);

	if (handle == INVALID_HANDLE_VALUE)
	{
		EMS_API_MESSAGE report;
		EmsInitializeMessage(&report, EmsApiSendDebugStatus);
		report.Client.SetDebugStatus.Message = L"Failed open process";
		report.Client.SetDebugStatus.Warning.Win32 = GetLastError();
	}

	return 0;
}



