#include "include/emstransport.h"
#include "include/emsmsg.h"

EMS_STATUS EmspSendMessageWithAlpc(
	_In_ HANDLE ConnectionHandle,
	_Inout_ PEMS_API_MESSAGE Message
)
{
	return E_NOTIMPL;
}

EMS_STATUS EmspSendMessageWithPipes(
	_In_ HANDLE ConnectionHandle,
	_Inout_ PEMS_API_MESSAGE Message
)
{
	if (SUCCEEDED(EmsValidateMessage(Message)))
		return E_INVALIDARG;

	DWORD bytesProcessed;

	//r
	if (!WriteFile(ConnectionHandle, Message, sizeof(EMS_API_MESSAGE), &bytesProcessed, NULL))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (bytesProcessed != sizeof(EMS_API_MESSAGE))
		return E_FAIL;

	//w
	if (!ReadFile(ConnectionHandle, Message, sizeof(EMS_API_MESSAGE), &bytesProcessed, NULL))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (bytesProcessed != sizeof(EMS_API_MESSAGE))
		return E_FAIL;

	return S_OK;
}

EMS_STATUS EmspConnectWithAlpc(
	_Inout_ PHANDLE ConnectionHandle
)
{
	return E_NOTIMPL;
}

EMS_STATUS EmspConnectWithPipes(
	_Inout_ PHANDLE ConnectionHandle
)
{
	while (TRUE)
	{
		*ConnectionHandle = CreateFileW(
			SYSX_PIPE_ENDPOINT,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL
		);

		if (*ConnectionHandle != INVALID_HANDLE_VALUE)
			break;

		if (GetLastError() != ERROR_PIPE_BUSY)
			return HRESULT_FROM_WIN32(GetLastError());

		if (!WaitNamedPipeW(
			SYSX_PIPE_ENDPOINT,
			SYSX_DEFAULT_TIMEOUT
		))
		{
			return E_FAIL;
		}
	}
}