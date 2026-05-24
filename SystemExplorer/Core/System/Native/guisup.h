#pragma once

#include "se.h"


#ifdef __cplusplus
extern "C" {
#endif


_Success_(return)
BOOLEAN SeExtractIcon(
	_In_ PCWSTR FileName,
	_Out_opt_ HICON * IconLarge,
	_Out_opt_ HICON * IconSmall
);

NTSTATUS SeExtractIconEx(
    _In_ PCWSTR FileName,
    _In_ BOOLEAN NativeFileName,
    _In_ LONG IconIndex,
    _In_ LONG IconLargeWidth,
    _In_ LONG IconLargeHeight,
    _In_ LONG IconSmallWidth,
    _In_ LONG IconSmallHeight,
    _Out_opt_ HICON* IconLarge,
    _Out_opt_ HICON* IconSmall
);

NTSTATUS SeIsInteractiveUserSession(
	VOID
);

NTSTATUS SeGetUserObjectInformation(
    _In_ HANDLE Handle,
    _In_ LONG Index,
    _Out_writes_bytes_opt_(UserObjectInformationLength) PVOID UserObjectInformation,
    _In_ ULONG UserObjectInformationLength,
    _Out_opt_ PULONG ReturnLength
);

#ifdef __cplusplus
}
#endif
