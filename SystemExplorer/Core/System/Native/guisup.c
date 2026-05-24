#include "guisup.h"
#include "sebasesup.h"
#include "mapldr.h"

#pragma comment(lib, "comctl32.lib")

_Success_(return)
BOOLEAN SeExtractIcon(
	_In_ PCWSTR FileName,
	_Out_opt_ HICON *IconLarge,
	_Out_opt_ HICON *IconSmall
)
{
	static SE_INITONCE initOnce = SE_INITONCE_INIT;
	static LONG(WINAPI *PrivateExtractIconExW)(
		_In_ PCWSTR FileName,
		_In_ LONG InconIndex,
		_Out_opt_ HICON *IconLarge,
		_Out_opt_ HICON *IconSmall,
		_In_ LONG IconCount
	) = NULL;

	HICON iconLarge = NULL;
	HICON iconSmall = NULL;

	if (SeBeginInitOnce(&initOnce))
	{
		PrivateExtractIconExW = SeGetDllBaseProcedureAddressZ(L"user32.dll", "PrivateExtractIconExW", 0);
		SeEndInitOnce(&initOnce);
	}

	if (!PrivateExtractIconExW)
		return FALSE;

	if (PrivateExtractIconExW(
		FileName,
		0,
		IconLarge ? &iconLarge : NULL,
		IconSmall ? &iconSmall : NULL,
		1
	) > 0)
	{
		if (IconLarge)
			*IconLarge = iconLarge;
		if(IconSmall)
			*IconSmall = iconSmall;

		return TRUE;
	}

	if (iconLarge)
		DestroyIcon(iconLarge);
	if (iconSmall)
		DestroyIcon(iconSmall);

	return FALSE;
}

NTSTATUS SeIsInteractiveUserSession(
	VOID
)
{
	NTSTATUS status;
	USEROBJECTFLAGS flags;

	memset(&flags, 0, sizeof(USEROBJECTFLAGS));

	status = SeGetUserObjectInformation(
		GetProcessWindowStation(),
		UOI_FLAGS,
		&flags,
		sizeof(USEROBJECTFLAGS),
		NULL
	);

	if (NT_SUCCESS(status))
	{
		if (BooleanFlagOn(flags.dwFlags, WSF_VISIBLE))
			return STATUS_SUCCESS;

		return STATUS_NOT_GUI_PROCESS;
	}

	return status;
}

NTSTATUS SeGetUserObjectInformation(
	_In_ HANDLE Handle,
	_In_ LONG Index,
	_Out_writes_bytes_opt_(UserObjectInformationLength) PVOID UserObjectInformation,
	_In_ ULONG UserObjectInformationLength,
	_Out_opt_ PULONG ReturnLength
)
{
	if (GetUserObjectInformation(
		Handle,
		Index,
		UserObjectInformation,
		UserObjectInformationLength,
		ReturnLength
	))
	{
		return STATUS_SUCCESS;
	}

	return SeGetLastWin32ErrorAsNtStatus();
}
