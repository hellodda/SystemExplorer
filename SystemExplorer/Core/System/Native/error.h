/*
 * Copyright (c) 2026 SystemExplorer, All rights reserved.
 *
 * This file is part of System Explorer.
 *
 * Authors:
 *
 *     hellodda 2026
 *
 *
 */

#pragma once
#include "se.h"

static ULONG LastErrorValue;

#ifdef __cplusplus
extern "C" {
#endif

NTSTATUS SeDosErrorToNtStatus(
    _In_ ULONG DosError
);

ULONG SeNtStatusToDosError(
    _In_ NTSTATUS Status
);

FORCEINLINE
ULONG SeGetLastError(
    VOID
)
{
    return NtReadCurrentTebUlong(FIELD_OFFSET(TEB, LastErrorValue)); // NtCurrentTeb()->LastErrorValue
}

FORCEINLINE
NTSTATUS SeGetLastWin32ErrorAsNtStatus(
    VOID
)
{
    return SeDosErrorToNtStatus(GetLastError());
}

#ifdef __cplusplus
}
#endif
