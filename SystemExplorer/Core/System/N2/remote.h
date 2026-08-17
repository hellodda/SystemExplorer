#pragma once
#include <string>
#include <wtsapi32.h>

typedef struct _SYSX_REMOTE_DEVICE
{
	HANDLE WTSHandle;
	HANDLE SCMHandle;
} SYSX_REMOTE_DEVICE, *PSYSX_REMOTE_DEVICE;

inline HRESULT SYSX_IMPL_ConnectToRemoteDevice(
	_Out_opt_ PSYSX_REMOTE_DEVICE device,
	_In_ std::wstring_view ip
)
{
	device->WTSHandle = WTSOpenServerExW(
		const_cast<LPWSTR>(ip.data())
	);

	if (!device->WTSHandle)
		return HRESULT_FROM_WIN32(GetLastError());
	return S_OK;
}

inline HRESULT SYSX_IMPL_CloseRemoteDevice(
	_In_ PSYSX_REMOTE_DEVICE device
)
{
	if (!device)
		return S_FALSE;

	if (device->WTSHandle)
		WTSCloseServer(device->WTSHandle);
}