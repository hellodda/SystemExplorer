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

NTSTATUS SeOpenProcess(
    _Out_ PHANDLE ProcessHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ HANDLE ProcessId
);

NTSTATUS SeOpenProcessToken(
    _In_ HANDLE ProcessHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _Out_ PHANDLE TokenHandle
);

NTSTATUS SeGetProcessBasicInformation(
    _In_ HANDLE ProcessHandle,
    _Out_ PPROCESS_BASIC_INFORMATION BasicInformation
);

NTSTATUS SeGetProcessExtendedBasicInformation(
    _In_ HANDLE ProcessHandle,
    _Out_ PPROCESS_EXTENDED_BASIC_INFORMATION ExtendedBasicInformation
);

NTSTATUS SeGetProcessImageFileNameWin32(
    _In_ HANDLE ProcessHandle,
    _Out_ PWSTR* FileName
);

NTSTATUS SeGetProcessProtection(
    _In_ HANDLE ProcessHandle,
    _Out_ PPS_PROTECTION Protection
);

NTSTATUS SeGetProcessMitigationPolicy(
    _In_ HANDLE ProcessHandle,
    _In_ PROCESS_MITIGATION_POLICY Policy,
    _Out_ PPROCESS_MITIGATION_POLICY_INFORMATION MitigationPolicy
);

NTSTATUS SeGetProcessPowerThrottlingState(
    _In_ HANDLE ProcessHandle,
    _Out_ PPOWER_THROTTLING_PROCESS_STATE PowerThrottlingState
);

NTSTATUS SeSetProcessPowerThrottlingState(
    _In_ HANDLE ProcessHandle,
    _In_ ULONG ControlMask,
    _In_ ULONG StateMask
);

NTSTATUS SeIsProcessEfficiencyModeEnabled(
    _In_ HANDLE ProcessHandle,
    _Out_ PBOOLEAN EfficiencyMode
);

NTSTATUS SeSuspendProcess(
    _In_ HANDLE ProcessHandle
);

NTSTATUS SeResumeProcess(
    _In_ HANDLE ProcessHandle
);

#ifdef __cplusplus
}
#endif