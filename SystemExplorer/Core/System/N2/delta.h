#pragma once
#ifndef SYSX_Delta_H
#define SYSX_Delta_H

typedef struct _SYSX_SINGLE_DELTA
{
    FLOAT Value;
    FLOAT Delta;
} SYSX_SINGLE_DELTA, * PSYSX_SINGLE_DELTA;

typedef struct _SYSX_DOUBLE_DELTA
{
    DOUBLE Value;
    DOUBLE Delta;
} SYSX_DOUBLE_DELTA, * PSYSX_DOUBLE_DELTA;

typedef struct _SYSX_UINT32_DELTA
{
    ULONG Value;
    ULONG Delta;
} SYSX_UINT32_DELTA, * PSYSX_UINT32_DELTA;

typedef struct _SYSX_UINT64_DELTA
{
    ULONG64 Value;
    ULONG64 Delta;
} SYSX_UINT64_DELTA, * PSYSX_UINT64_DELTA;

typedef struct _SYSX_UINTPTR_DELTA
{
    ULONG_PTR Value;
    ULONG_PTR Delta;
} SYSX_UINTPTR_DELTA, * PSYSX_UINTPTR_DELTA;

#define SxInitializeDelta(DltMgr) \
    ((DltMgr)->Value = 0, (DltMgr)->Delta = 0)

#define SxUpdateDelta(DltMgr, NewValue) \
    ((DltMgr)->Delta = (NewValue) - (DltMgr)->Value, \
    (DltMgr)->Value = (NewValue), (DltMgr)->Delta)

#define SYSX_SINGLE_DELTA_INIT { 0.0F, 0.0F }
#define SYSX_DOUBLE_DELTA_INIT { 0.0, 0.0 }
#define SYSX_UINT32_DELTA_INIT { 0UL, 0UL }
#define SYSX_UINT64_DELTA_INIT { 0ULL, 0ULL }
#define SYSX_UINTPTR_DELTA_INIT { 0, 0 }

#endif // SYSX_Delta_H