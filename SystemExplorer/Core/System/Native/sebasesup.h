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

#ifdef __cplusplus
extern "C" {
#endif

HANDLE PhHeapHandle = NULL;

_Use_decl_annotations_
PVOID SeAllocate(
    _In_ SIZE_T Size
);

FORCEINLINE
PVOID SeAllocateCopy(
    _In_ PVOID Data,
    _In_ SIZE_T Size
)
{
    PVOID copy;

    copy = SeAllocate(Size);
    memcpy(copy, Data, Size);

    return copy;
}

_Use_decl_annotations_
VOID SeFree(
    _In_opt_ _Frees_ptr_opt_ _Post_invalid_ PVOID Memory
);
 
#ifdef __cplusplus
}
#endif