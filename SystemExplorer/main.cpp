#include "pch.h"
#include "App.xaml.h"
#include "resource.h"
#include "Helpers/Win32Helper.h"
#include "Helpers/Common.h"

#include "Core/System/native.h"
#include "Core/AI/Actions/ProcessesActionProvider.h"

VOID SepEnablePrivileges(
    VOID
)
{
    HANDLE tokenHandle;

    if (NT_SUCCESS(SeOpenProcessToken(
        NtCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES,
        &tokenHandle
    )))
    {
        const LUID_AND_ATTRIBUTES privileges[] =
        {
            { RtlConvertUlongToLuid(SE_DEBUG_PRIVILEGE), SE_PRIVILEGE_ENABLED },
            { RtlConvertUlongToLuid(SE_INC_BASE_PRIORITY_PRIVILEGE), SE_PRIVILEGE_ENABLED },
            { RtlConvertUlongToLuid(SE_INC_WORKING_SET_PRIVILEGE), SE_PRIVILEGE_ENABLED },
            { RtlConvertUlongToLuid(SE_LOAD_DRIVER_PRIVILEGE), SE_PRIVILEGE_ENABLED },
            { RtlConvertUlongToLuid(SE_PROF_SINGLE_PROCESS_PRIVILEGE), SE_PRIVILEGE_ENABLED },
            { RtlConvertUlongToLuid(SE_BACKUP_PRIVILEGE), SE_PRIVILEGE_ENABLED },
            { RtlConvertUlongToLuid(SE_RESTORE_PRIVILEGE), SE_PRIVILEGE_ENABLED },
            { RtlConvertUlongToLuid(SE_SHUTDOWN_PRIVILEGE), SE_PRIVILEGE_ENABLED },
            { RtlConvertUlongToLuid(SE_TAKE_OWNERSHIP_PRIVILEGE), SE_PRIVILEGE_ENABLED },
            { RtlConvertUlongToLuid(SE_SECURITY_PRIVILEGE), SE_PRIVILEGE_ENABLED },
        };
        UCHAR privilegesBuffer[FIELD_OFFSET(TOKEN_PRIVILEGES, Privileges) + sizeof(privileges)];
        PTOKEN_PRIVILEGES tokenPrivileges;

        tokenPrivileges = (PTOKEN_PRIVILEGES)privilegesBuffer;
        tokenPrivileges->PrivilegeCount = RTL_NUMBER_OF(privileges);
        memcpy(tokenPrivileges->Privileges, privileges, sizeof(privileges));

        NtAdjustPrivilegesToken(
            tokenHandle,
            FALSE,
            tokenPrivileges,
            0,
            NULL,
            NULL
        );

        NtClose(tokenHandle);
    }
}

INT APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ INT nCmdShow
)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    winrt::init_apartment(winrt::apartment_type::single_threaded);

    SepEnablePrivileges();

#ifdef DEBUG_E
    RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, nullptr);
#endif 

    winrt::Microsoft::UI::Xaml::Application::Start([](auto&&)
    {
        winrt::make<winrt::SystemExplorer::implementation::App>();
    });

    return 0;
}