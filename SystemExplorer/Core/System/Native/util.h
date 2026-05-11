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

typedef struct _VS_VERSION_INFO_STRUCT16
{
    USHORT Length;
    USHORT ValueLength;
    CHAR Key[1];
} VS_VERSION_INFO_STRUCT16, * PVS_VERSION_INFO_STRUCT16;

#ifdef __cplusplus
extern "C" {
#endif

FORCEINLINE
BOOLEAN SeIsFileVersionInfo32(
    _In_ PVOID VersionInfo
)
{
    return ((PVS_VERSION_INFO_STRUCT16)VersionInfo)->Key[0] < 32;
}


NTSTATUS SeGetFileVersionInfo(
    _In_ PCWSTR FileName,
    _Out_ PVOID* VersionInfo
);

#ifdef __cplusplus
}
#endif