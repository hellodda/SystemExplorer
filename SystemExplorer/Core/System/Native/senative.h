/*
 * Copyright (c) 2026 SystemExplorer, All rights reserved.
 *
 * This file is part of System Explorer.
 *
 * Authors:
 *
 *     hellodda 2026
 *
 */

#pragma once
#include "se.h"

#ifdef __cplusplus
extern "C" {
#endif

NTSTATUS SeGuardGrantSuppressedCallAccess(
    _In_ HANDLE ProcessHandle,
    _In_ PVOID VirtualAddress
);

FORCEINLINE
NTSTATUS SeAcquireLoaderLock(
    VOID
)
{
    return RtlEnterCriticalSection(NtCurrentPeb()->LoaderLock);
}

FORCEINLINE
NTSTATUS SeReleaseLoaderLock(
    VOID
)
{
    return RtlLeaveCriticalSection(NtCurrentPeb()->LoaderLock);
}

#ifdef __cplusplus
}
#endif
