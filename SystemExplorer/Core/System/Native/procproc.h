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
#include "dltmgr.h"

typedef struct _SE_PROCESS_RECORD* PSE_PROCESS_RECORD;

typedef struct _SE_PROCESS_ITEM
{
	ULONG State;
	PSE_PROCESS_RECORD Record;

	// basic

	HANDLE ProcessId;
	HANDLE ParentProcessId;
	PCWSTR ProcessName;
	ULONG SessionId;
	LARGE_INTEGER CreateTime;

	// handles

	HANDLE QueryHandle;

	// parameters

	PCWSTR FileName;
	PCWSTR CommandLine;

	// file

	ULONG_PTR SmallIconIndex;
	ULONG_PTR LargeIconIndex;
	// PH_IMAGE_VERSION_INFO VersionInfo;

	// security

	PSID Sid;
	TOKEN_ELEVATION_TYPE ElevationType;
	PWSTR IntegrityString;
	PS_PROTECTION Protection;

	// other

	HANDLE ConsoleHostProcessId;
	ULONGLONG ProcessStartKey;
	ULONGLONG CreateInterruptTime;
	ULONGLONG SessionCreateTime;
	ULONG ImageChecksum;
	ULONG ImageTimeStamp;

	// signature, packed

	ULONG VerifyResult;
	PWSTR VerifySignerName;
	ULONG ImportFunctions;
	ULONG ImportModules;

	union
	{
		ULONG Flags;
		struct
		{
			ULONG UpdateIsDotNet : 1;
			ULONG IsBeingDebugged : 1;
			ULONG IsDotNet : 1;
			ULONG IsElevated : 1;
			ULONG IsInJob : 1;
			ULONG IsInSignificantJob : 1;
			ULONG IsPacked : 1;
			ULONG IsHandleValid : 1;
			ULONG IsSuspended : 1;
			ULONG IsWow64Process : 1;
			ULONG IsImmersive : 1;
			ULONG IsPartiallySuspended : 1;
			ULONG IsProtectedHandle : 1;
			ULONG IsProtectedProcess : 1;
			ULONG IsSecureProcess : 1;
			ULONG IsSubsystemProcess : 1;
			ULONG IsPackagedProcess : 1;
			ULONG IsBackgroundProcess : 1;
			ULONG IsCrossSessionProcess : 1;
			ULONG IsSnapshotProcess : 1;
			ULONG IsFrozenProcess : 1;
			ULONG IsUIAccessEnabled : 1;
			ULONG IsControlFlowGuardEnabled : 1;
			ULONG IsCetEnabled : 1;
			ULONG IsXfgEnabled : 1;
			ULONG IsXfgAuditEnabled : 1;
			ULONG IsPowerThrottling : 1;
			ULONG IsSystemProcess : 1;
			ULONG IsSecureSystem : 1;
			ULONG Spare : 3;
		};
	};

	volatile LONG JustProcessed;

	KPRIORITY BasePriority;
	union
	{
		KAFFINITY AffinityMaskSingle; // Single processor group
		PKAFFINITY AffinityMaskGroups; // Multiple processor groups * PhSystemProcessorInformation.NumberOfProcessorGroups
	};
	ULONG AffinityPopulationCount;
	ULONG PriorityClass;
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	ULONG NumberOfHandles;
	ULONG NumberOfThreads;

	FLOAT CpuUsage; // Below Windows 7, sum of kernel and user CPU usage; above Windows 7, cycle-based CPU usage.
	FLOAT CpuKernelUsage;
	FLOAT CpuUserUsage;
	FLOAT CpuAverageUsage;

	SE_UINT64_DELTA CpuKernelDelta;
	SE_UINT64_DELTA CpuUserDelta;
	SE_UINT64_DELTA IoReadDelta;
	SE_UINT64_DELTA IoWriteDelta;
	SE_UINT64_DELTA IoOtherDelta;
	SE_UINT64_DELTA IoReadCountDelta;
	SE_UINT64_DELTA IoWriteCountDelta;
	SE_UINT64_DELTA IoOtherCountDelta;
	SE_UINT64_DELTA ContextSwitchesDelta;
	SE_UINT32_DELTA PageFaultsDelta;
	SE_UINT32_DELTA HardFaultsDelta;
	SE_UINT64_DELTA CycleTimeDelta; // since WIN7

	VM_COUNTERS_EX VmCounters;
	IO_COUNTERS IoCounters;
	ULONGLONG WorkingSetPrivateSize; // since VISTA
	ULONG PeakNumberOfThreads; // since WIN7
	ULONG HardFaultCount; // since WIN7
} SE_PROCESS_ITEM, * PSE_PROCESS_ITEM;

typedef struct _SE_PROCESS_RECORD
{
	LIST_ENTRY ListEntry;
	LONG RefCount;
	ULONG Flags;

	HANDLE ProcessId;
	HANDLE ParentProcessId;
	ULONG SessionId;
	ULONGLONG ProcessSequenceNumber;
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER ExitTime;

	PWSTR ProcessName;
	PWSTR FileName;
	PWSTR CommandLine;
	PWSTR UserName;
} SE_PROCESS_RECORD, * PSE_PROCESS_RECORD;