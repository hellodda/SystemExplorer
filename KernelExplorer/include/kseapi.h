#pragma once
#pragma once
#include <ntddk.h>
#include <Windows.h>
#include <sal.h>

#ifdef _KERNEL_MODE
#define PHNT_MODE PHNT_MODE_KERNEL
#endif 
#pragma warning(push)
#pragma warning(disable : 4201)

typedef _Enum_is_bitflag_ enum _KSE_PROCESS_STATE
{

} KSE_PROCESS_STATE;

typedef enum _KSE_PROCESS_INFORMATION_CLASS
{
    KseProcessBasicInformation,
    KseProcessStateInformation
} KSE_PROCESS_INFORMATION_CLASS;

typedef struct _KSE_PROCESS_BASIC_INFORMATION
{
    KSE_PROCESS_STATE ProcessState;

    ULONG ProcessStartKey;
    ULONG UserWritableReferences;
    CLIENT_ID CreatorClientId;
    SIZE_T NumberOfImageLoads;

    union
    {
        ULONG Flags;
        struct
        {
            ULONG CreateNotification : 1;
            ULONG ExitNotification : 1;
            ULONG VerifiedProcess : 1;
            ULONG SecurelyCreated : 1;
            ULONG Protected : 1;
            ULONG IsWow64 : 1;
            ULONG IsSubsystemProcess : 1;
            ULONG AllocatedImageName : 1;
            ULONG Reserved : 24;
        };
    };

    SIZE_T NumberOfThreads;
    ACCESS_MASK ProcessAllowedMask;
    ACCESS_MASK ThreadAllowedMask;
    SIZE_T NumberOfMicrosoftImageLoads;
    SIZE_T NumberOfAntimalwareImageLoads;
    SIZE_T NumberOfVerifiedImageLoads;
    SIZE_T NumberOfUntrustedImageLoads;
} KSE_PROCESS_BASIC_INFORMATION, * PKSE_PROCESS_BASIC_INFORMATION;

typedef struct _KSE_PROCESS_HANDLE
{
    HANDLE Handle;
    PVOID Object;
    ACCESS_MASK GrantedAccess;
    USHORT ObjectTypeIndex;
    USHORT Reversed1;
    ULONG HandleAttributes;
    ULONG Reversed2;
} KSE_PROCESS_HANDLE, * PKSE_PROCESS_HANDLE;

typedef struct _KSE_MESSAGE_TIMEOUTS
{
    LARGE_INTEGER AsyncTimeout;
    LARGE_INTEGER DefaultTimeout;
    LARGE_INTEGER ProcessCreateTimeout;
    LARGE_INTEGER FilePreCreateTimeout;
    LARGE_INTEGER FilePostCreateTimeout;
} KSE_MESSAGE_TIMEOUTS, * PKSE_MESSAGE_TIMEOUTS;