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

#ifdef __cplusplus
extern "C" {
#endif

NTSTATUS SeGetLoaderEntryImageNtHeaders(
    _In_ PVOID BaseAddress,
    _Out_ PIMAGE_NT_HEADERS* ImageNtHeaders
);

NTSTATUS SeGetLoaderEntryImageDirectory(
    _In_ PVOID BaseAddress,
    _In_ PIMAGE_NT_HEADERS ImageNtHeader,
    _In_ ULONG ImageDirectoryIndex,
    _Out_ PIMAGE_DATA_DIRECTORY* ImageDataDirectoryEntry,
    _Out_ PVOID* ImageDirectoryEntry,
    _Out_opt_ SIZE_T* ImageDirectoryLength
);

BOOLEAN SeLoaderEntryImageExportSupressionPresent(
    _In_ PVOID BaseAddress,
    _In_ PIMAGE_NT_HEADERS ImageNtHeader
);

PVOID SeGetLoaderEntryImageExportFunction(
    _In_ PVOID BaseAddress,
    _In_ PIMAGE_NT_HEADERS ImageNtHeader,
    _In_ PIMAGE_DATA_DIRECTORY DataDirectory,
    _In_ PIMAGE_EXPORT_DIRECTORY ExportDirectory,
    _In_opt_ PCSTR ExportName,
    _In_opt_ USHORT ExportOrdinal
);

PLDR_DATA_TABLE_ENTRY SeFindLoaderEntryNameHash(
    _In_ ULONG BaseNameHash
);

_Use_decl_annotations_
PVOID SeGetLoaderEntryDllBase(
    _In_opt_ PCWSTR FullDllName,
    _In_opt_ PCWSTR BaseDllName
);

FORCEINLINE
PVOID
NTAPI
SeGetLoaderEntryDllBaseZ(
    _In_ PCWSTR DllName
)
{
    return SeGetLoaderEntryDllBase(NULL, DllName);
}

PVOID SeGetDllBaseProcedureAddress(
    _In_ PVOID DllBase,
    _In_opt_ PCSTR ProcedureName,
    _In_opt_ USHORT ProcedureNumber
);

PVOID SeLoadLibrary(
    _In_ PCWSTR FileName
);

VOID SeLoaderEntryGrantSuppressedCall(
    _In_ PVOID ExportAddress
);

NTSTATUS SeLoadResource(
    _In_ PVOID DllBase,
    _In_ PCWSTR Name,
    _In_ PCWSTR Type,
    _Out_opt_ ULONG* ResourceLength,
    _Out_opt_ PVOID* ResourceBuffer
);

NTSTATUS SeLoadResourceCopy(
    _In_ PVOID DllBase,
    _In_ PCWSTR Name,
    _In_ PCWSTR Type,
    _Out_opt_ ULONG* ResourceLength,
    _Out_opt_ PVOID* ResourceBuffer
);

#ifdef __cplusplus
}
#endif