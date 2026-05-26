#include "pch.h"
#include "App.xaml.h"
#include "Core/System/native.h"
#include "Helpers/Common.h"
#include <CommCtrl.h>
#include <shlobj.h>
#include <winsta.h>
#include <stacktrace>
#include <Core/System/Native/util.h>
#include <Core/System/Native/senative.h>
#include <winrt/Microsoft.Windows.AppLifecycle.h>
#include <winrt/Microsoft.Windows.Storage.h>

#include "Core/Diagnostics/AsyncFileLogger.h"

// potom uberu v manifest
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define DEBUG
//#define SE_MINIMAL_ERRORMODE
//#define DEBUG_E

#pragma region Error Reporting

typedef enum _TRIAGE_DUMP_TYPE
{
    TriageDumpTypeMinimal,
    TriageDumpTypeNormal,
    TriageDumpTypeFull,
} TRIAGE_DUMP_TYPE;

static LPTOP_LEVEL_EXCEPTION_FILTER PreviousUnhandledExceptionFilter{ NULL };

void CreateUnhandledExceptionCrashDump(
    _In_ PEXCEPTION_POINTERS ExceptionInfo,
    _In_ TRIAGE_DUMP_TYPE DumpType
)
{
    std::wstring baseDir;
    baseDir = SeGetCurrentAppXPath();
    baseDir += L"\\CrashDump";

    std::wstring win32CreatePath = baseDir;
    if (win32CreatePath.size() > 2 && win32CreatePath[1] == L':')
    {
        win32CreatePath = L"\\\\?\\" + win32CreatePath;
    }
    THROW_IF_WIN32_BOOL_FALSE(CreateDirectoryW(win32CreatePath.c_str(), NULL));

    std::wstring fileName = baseDir + L"\\SystemExplorer_" + SeRandomString(9) + L"_Dump.dmp";
    if (fileName.size() > 2 && fileName[1] == L':')
    {
        fileName = L"\\\\?\\" + fileName;
    }

    wil::unique_hfile fileHandle{ CreateFileW(
        fileName.c_str(),
        GENERIC_WRITE,
        FILE_SHARE_WRITE,
        NULL,
        CREATE_ALWAYS,          
        FILE_ATTRIBUTE_NORMAL,
        NULL
    ) };

    if (fileHandle.is_valid())
    {
        MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
        ULONG dumpType{ TriageDumpTypeMinimal };

        exceptionInfo.ThreadId = HandleToUlong(NtCurrentThreadId());
        exceptionInfo.ExceptionPointers = ExceptionInfo;
        exceptionInfo.ClientPointers = TRUE;

        switch (DumpType)
        {
        case TriageDumpTypeMinimal:
            dumpType =
                MiniDumpWithDataSegs |
                MiniDumpWithUnloadedModules |
                MiniDumpWithProcessThreadData |
                MiniDumpWithThreadInfo |
                MiniDumpIgnoreInaccessibleMemory;
            break;
        case TriageDumpTypeNormal:
            dumpType =
                MiniDumpWithDataSegs |
                MiniDumpWithHandleData |
                MiniDumpScanMemory |
                MiniDumpWithUnloadedModules |
                MiniDumpWithProcessThreadData |
                MiniDumpWithFullMemoryInfo |
                MiniDumpWithThreadInfo |
                MiniDumpIgnoreInaccessibleMemory |
                MiniDumpWithTokenInformation;
            break;
        case TriageDumpTypeFull:
            dumpType =
                MiniDumpWithDataSegs |
                MiniDumpWithFullMemory |
                MiniDumpWithHandleData |
                MiniDumpWithUnloadedModules |
                MiniDumpWithIndirectlyReferencedMemory |
                MiniDumpWithProcessThreadData |
                MiniDumpWithFullMemoryInfo |
                MiniDumpWithThreadInfo |
                MiniDumpIgnoreInaccessibleMemory |
                MiniDumpWithTokenInformation |
                MiniDumpWithAvxXStateContext;
            break;
        }

        SeWriteMiniDumpProcess(
            NtCurrentProcess(),
            NtCurrentProcessId(),
            fileHandle.get(),
            static_cast<MINIDUMP_TYPE>(dumpType),
            &exceptionInfo,
            NULL,
            NULL
        );
    }
    else
      THROW_WIN32(GetLastError());
}

LONG CALLBACK UnhandledExceptionCallback(
    _In_ PEXCEPTION_POINTERS ExceptionInfo
)
{
    std::wstring errorMessage;
    std::wstring message;

#ifdef DEBUG
    std::stacktrace trace = std::stacktrace::current(1);
    std::string stacktrace = std::to_string(trace);
#endif // DEBUG


    LONG result;
    if (SeIsDebuggerPresent())
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    if (NT_SUCCESS(SeIsInteractiveUserSession()))
    {
        TASKDIALOGCONFIG config = { sizeof(TASKDIALOGCONFIG) };
        TASKDIALOG_BUTTON buttons[6] =
        {
            { 101, L"Full\nA complete dump of the process, rarely needed most of the time." },
            { 102, L"Normal\nFor most purposes, this dump file is the most useful." },
            { 103, L"Minimal\nA very limited dump with limited data." },
            { 104, L"Restart\nRestart the application." },
            { 105, L"Ignore" },
            { 106, L"Exit " },
        };

        //if (NT_NTWIN32(ExceptionInfo->ExceptionRecord->ExceptionCode))
        //    errorMessage = SeGetStatusMessage(0, SeNtStatusToDosError(ExceptionInfo->ExceptionRecord->ExceptionCode));
        //else
        //    errorMessage = SeGetStatusMessage(ExceptionInfo->ExceptionRecord->ExceptionCode, 0);

        message = std::format(
            L"0x{:08X} ({})",
            ExceptionInfo->ExceptionRecord->ExceptionCode,
            errorMessage
        );

        config.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION | TDF_USE_COMMAND_LINKS | TDF_EXPAND_FOOTER_AREA;
        config.pszWindowTitle = L"System Explorer InDev";
        config.pszMainIcon = TD_ERROR_ICON;
        config.pszMainInstruction = L"System Explorer has crashed :(";
        config.cButtons = RTL_NUMBER_OF(buttons);
        config.pButtons = buttons;
        config.nDefaultButton = 106;
        config.cxWidth = 250;
        config.pszContent = message.c_str();
#ifdef DEBUG
        config.pszExpandedInformation = std::wstring(stacktrace.begin(), stacktrace.end()).c_str();
#endif

        if (SeShowTaskDialog(&config, (ULONG*)&result, NULL, NULL))
        {
            switch (result)
            {
            case 101:
                CreateUnhandledExceptionCrashDump(ExceptionInfo, TriageDumpTypeFull);
                break;
            case 102:
                CreateUnhandledExceptionCrashDump(ExceptionInfo, TriageDumpTypeNormal);
                break;
            case 103:
                CreateUnhandledExceptionCrashDump(ExceptionInfo, TriageDumpTypeMinimal);
                break;
            case 104:
            {
                using namespace winrt::Microsoft::Windows::AppLifecycle;

                AppInstance::Restart(L"");
            }
            break;
            case 105:
            {
                return EXCEPTION_CONTINUE_EXECUTION;
            }
            break;
            }
        }
        else
        {

        }
    }
    else
    {
        ULONG response;
        std::wstring title;

        /*     if (NT_NTWIN32(ExceptionInfo->ExceptionRecord->ExceptionCode))
                 errorMessage = SeGetStatusMessage(0, SeNtStatusToDosError(ExceptionInfo->ExceptionRecord->ExceptionCode));
             else
                 errorMessage = SeGetStatusMessage(ExceptionInfo->ExceptionRecord->ExceptionCode, 0);*/

        title = L"System Informer has crashed :(";

#ifdef DEBUG
        message = std::format(
            L"{}\r\n0x{:08X} ({})\r\n{}",
            title,
            ExceptionInfo->ExceptionRecord->ExceptionCode,
            errorMessage,
            std::wstring(stacktrace.begin(), stacktrace.end())
        );
#else
        message = std::format(
            L"{}\r\n0x{:08X} ({})",
            title,
            ExceptionInfo->ExceptionRecord->ExceptionCode,
            errorMessage
        );
#endif
        //if (WinStationSendMessageW(
        //    SERVERNAME_CURRENT,
        //    USER_SHARED_DATA->ActiveConsoleId, // RtlGetActiveConsoleId
        //    title.data(),
        //    (ULONG)title.size(),
        //    message.data(),
        //    (ULONG)message.size(),
        //    MB_OKCANCEL | MB_ICONERROR,
        //    30,
        //    &response,
        //    FALSE
        //))
        //{

        //}
    }
    return PreviousUnhandledExceptionFilter(ExceptionInfo);
}

#pragma endregion

void InitializeLogger()
{
    auto appCachePath = winrt::Microsoft::Windows::Storage::ApplicationData::GetDefault().LocalCacheFolder().Path();

    winrt::SystemExplorer::Core::Diagnostics::AsyncFileLogger::Instance().Initialize(appCachePath.c_str() + std::wstring(L"\\SystemExplorer.log"));

    wil::SetResultLoggingCallback(WilResultLoggingCallBack);
}

void InitializeCommonControls()
{
    INITCOMMONCONTROLSEX icex;

    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC =
        ICC_LISTVIEW_CLASSES |
        ICC_TREEVIEW_CLASSES |
        ICC_BAR_CLASSES |
        ICC_TAB_CLASSES |
        ICC_PROGRESS_CLASS |
        ICC_COOL_CLASSES |
        ICC_STANDARD_CLASSES |
        ICC_LINK_CLASS
        ;

    THROW_IF_WIN32_BOOL_FALSE_MSG(
        InitCommonControlsEx(&icex),
        "Failed to initialize common controls."
    );
}

void InitializeExceptionPolicy()
{
#if SE_MINIMAL_ERRORMODE
    ULONG errorMode;

    if (NT_SUCCESS(SeGetProcessErrorMode(NtCurrentProcess(), &errorMode)))
    {
        ClearFlag(errorMode, SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

        LOG_IF_NTSTATUS_FAILED_MSG(
            SeSetProcessErrorMode(NtCurrentProcess(), errorMode),
            "Failed to set current process error mode to minimal"
        );
    }
#else
    THROW_IF_NTSTATUS_FAILED_MSG(
        SeSetProcessErrorMode(NtCurrentProcess(), 0),
        "Failed to set current process error mode to 0"
    );
#endif
    PreviousUnhandledExceptionFilter = SetUnhandledExceptionFilter(UnhandledExceptionCallback);
}

void EnablePrivileges()
{
	native::unique_nt_handle token;

    THROW_IF_FAILED(SeOpenProcessToken(
        NtCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES,
        &token
    ));

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
    PTOKEN_PRIVILEGES tokenPrivileges{};

    tokenPrivileges = (PTOKEN_PRIVILEGES)privilegesBuffer;
    tokenPrivileges->PrivilegeCount = RTL_NUMBER_OF(privileges);
    memcpy(tokenPrivileges->Privileges, privileges, sizeof(privileges));

    THROW_IF_NTSTATUS_FAILED_MSG(NtAdjustPrivilegesToken(
        token.get(),
        FALSE,
        tokenPrivileges,
        0,
        NULL,
        NULL
    ), "Failed to apply privileges");
}

#include "Helpers/UI/DesktopNotification.h"

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

    auto args = winrt::SystemExplorer::Helpers::UI::DesktopNotificationArgs{};
    args.Tag = L"xz";
    args.Title = L"test notification";
    args.Message = L"test message";

    winrt::SystemExplorer::Helpers::UI::DesktopNotification::SendNotification(args, nullptr);

    InitializeLogger();
    try
    {
        InitializeCommonControls();
        InitializeExceptionPolicy();
        EnablePrivileges();
    }
    CATCH_LOG()

#ifdef DEBUG_E
    RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, nullptr);
#endif 

    winrt::Microsoft::UI::Xaml::Application::Start([](auto&&)
    {
        winrt::make<winrt::SystemExplorer::implementation::App>();
    });

    return 0;
}
