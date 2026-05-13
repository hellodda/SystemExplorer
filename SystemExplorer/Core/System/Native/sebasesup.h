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
#include "sesup.h"

static HANDLE SeHeapHandle = NULL;

#ifdef __cplusplus
extern "C" {
#endif

ULONG64 SeReadTimeStampCounter(
    VOID
);

#define SE_EVENT_SET 0x1
#define SE_EVENT_SET_SHIFT 0
#define SE_EVENT_REFCOUNT_SHIFT 1
#define SE_EVENT_REFCOUNT_INC 0x2
#define SE_EVENT_REFCOUNT_MASK (((ULONG_PTR)1 << 15) - 1)

typedef struct _SE_EVENT
{
    union
    {
        ULONG_PTR Value;
        struct
        {
            USHORT Set : 1;
            USHORT RefCount : 15;
            UCHAR Reserved;
            UCHAR AvailableForUse;
            ULONG Spare;
        };
    };
    HANDLE EventHandle;
} SE_EVENT, * PSE_EVENT;

#define SE_EVENT_INIT { { SE_EVENT_REFCOUNT_INC }, NULL }

FORCEINLINE VOID SepReferenceEvent(
    _Inout_ PSE_EVENT Event
)
{
    _InterlockedExchangeAddPointer((PLONG_PTR)&Event->Value, SE_EVENT_REFCOUNT_INC);
}

FORCEINLINE VOID SepDereferenceEvent(
    _Inout_ PSE_EVENT Event,
    _In_opt_ HANDLE EventHandle
)
{
    ULONG_PTR value;

    value = _InterlockedExchangeAddPointer((PLONG_PTR)&Event->Value, -SE_EVENT_REFCOUNT_INC);

    if (((value >> SE_EVENT_REFCOUNT_SHIFT) & SE_EVENT_REFCOUNT_MASK) - 1 == 0)
    {
        if (EventHandle)
        {
            NtClose(EventHandle);
            Event->EventHandle = NULL;
        }
    }
}

BOOLEAN FASTCALL SefWaitForEvent(
    _Inout_ PSE_EVENT Event,
    _In_opt_ PLARGE_INTEGER Timeout
);

FORCEINLINE
BOOLEAN
SeWaitForEvent(
    _Inout_ PSE_EVENT Event,
    _In_opt_ PLARGE_INTEGER Timeout
)
{
    if (Event->Set)
        return TRUE;

    return SefWaitForEvent(Event, Timeout);
}

#define SE_INITONCE_SHIFT 31
#define SE_INITONCE_INITIALIZING (0x1 << SE_INITONCE_SHIFT)
#define SE_INITONCE_INITIALIZING_SHIFT SE_INITONCE_SHIFT

typedef struct _SE_INITONCE
{
    SE_EVENT Event;
} SE_INITONCE, * PSE_INITONCE;

#define SE_INITONCE_INIT { SE_EVENT_INIT }

BOOLEAN FASTCALL SefBeginInitOnce(
    _Inout_ PSE_INITONCE InitOnce
);

VOID FASTCALL SefSetEvent(
    _Inout_ PSE_EVENT Event
);

#define SeSetEvent SefSetEvent

VOID FASTCALL SefEndInitOnce(
    _Inout_ PSE_INITONCE InitOnce
);

#define SeEndInitOnce SefEndInitOnce

FORCEINLINE
BOOLEAN
SeBeginInitOnce(
    _Inout_ PSE_INITONCE InitOnce
)
{
    if (InitOnce->Event.Set)
        return FALSE;

    return SefBeginInitOnce(InitOnce);
}

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