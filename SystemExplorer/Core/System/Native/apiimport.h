/*
 * Copyright (c) 2026 SystemExplorer, All rights reserved.
 *
 * This file is part of System Explorer.
 *
 * Authors:
 *
 *     hellodda 2026
 *
 *	   System Explorer - SE
 */

#pragma once
#include "se.h"
#include <sddl.h>
#include <shlwapi.h>
#include <userenv.h>
#include <ntpsapi.h>


EXTERN_C_START

#define SE_DECLARE_IMPORT(Name) typeof(&(Name)) Name##_Import(VOID)

//SE_DECLARE_IMPORT(NtCreateThreadStateChange);
//SE_DECLARE_IMPORT(NtChangeProcessState);
//SE_DECLARE_IMPORT(NtCreateProcessStateChange);
//SE_DECLARE_IMPORT(NtSetInformationVirtualMemory);

EXTERN_C_END