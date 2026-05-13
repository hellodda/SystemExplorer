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

#ifndef _PH_DLTMGR_H
#define _PH_DLTMGR_H

typedef struct _SE_SINGLE_DELTA
{
    FLOAT Value;
    FLOAT Delta;
} SE_SINGLE_DELTA, * PSE_SINGLE_DELTA;

typedef struct _SE_DOUBLE_DELTA
{
    DOUBLE Value;
    DOUBLE Delta;
} SE_DOUBLE_DELTA, * PSE_DOUBLE_DELTA;

typedef struct _SE_UINT32_DELTA
{
    ULONG Value;
    ULONG Delta;
} SE_UINT32_DELTA, * PSE_UINT32_DELTA;

typedef struct _SE_UINT64_DELTA
{
    ULONG64 Value;
    ULONG64 Delta;
} SE_UINT64_DELTA, * PSE_UINT64_DELTA;

typedef struct _SE_UINTPTR_DELTA
{
    ULONG_PTR Value;
    ULONG_PTR Delta;
} SE_UINTPTR_DELTA, * PSE_UINTPTR_DELTA;

#define SeInitializeDelta(DltMgr) \
    ((DltMgr)->Value = 0, (DltMgr)->Delta = 0)

#define SeUpdateDelta(DltMgr, NewValue) \
    ((DltMgr)->Delta = (NewValue) - (DltMgr)->Value, \
    (DltMgr)->Value = (NewValue), (DltMgr)->Delta)

#define SE_SINGLE_DELTA_INIT { 0.0F, 0.0F }
#define SE_DOUBLE_DELTA_INIT { 0.0, 0.0 }
#define SE_UINT32_DELTA_INIT { 0UL, 0UL }
#define SE_UINT64_DELTA_INIT { 0ULL, 0ULL }
#define SE_UINTPTR_DELTA_INIT { 0, 0 }

#endif