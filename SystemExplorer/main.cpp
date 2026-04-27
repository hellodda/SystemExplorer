#include "pch.h"
#include "App.xaml.h"
#include "resource.h"
#include "Helpers/Win32Helper.h"
#include "Helpers/Common.h"

#include "Core/AI/Actions/ProcessesActionProvider.h"
#include "Core/Services/ComRegistrationService.h"



using namespace winrt::SystemExplorer::Helpers;
using namespace winrt::SystemExplorer::Core;

// potom pomenyayu
LONG WINAPI PlatformExceptionFilter(
    _In_ PEXCEPTION_POINTERS pExceptionInfo
)
{
    const auto* record = pExceptionInfo->ExceptionRecord;

    LOG_NTSTATUS_MSG(
        record->ExceptionCode,
        "Platform Exception Filter Caught Address: 0x%p, Flags: %lu",
        record->ExceptionAddress,
        record->ExceptionFlags
    );

    const auto errorDesc = Win32Helper::GetErrorMessage(record->ExceptionCode);
    const auto appName = Win32Helper::GetLocalizedResource(IDS_APP_NAME);

    auto details = Win32Helper::GetLocalizedResource(IDS_EXCEPTION_DETAILS);

    details += Format(Win32Helper::GetLocalizedResource(IDS_FAULTING_CODE).c_str(), record->ExceptionCode);
    details += Format(Win32Helper::GetLocalizedResource(IDS_DESCRIPTION).c_str(), errorDesc.c_str());
    details += Format(Win32Helper::GetLocalizedResource(IDS_INSTRUCTION_ADDRESS).c_str(), record->ExceptionAddress);

    const auto finalMessage = Format(
        Win32Helper::GetLocalizedResource(IDS_CRITICAL_ERROR).c_str(),
        details.c_str()
    );

    MessageBoxW(
        nullptr,
        finalMessage.c_str(),
        appName.c_str(),
        MB_OK | MB_ICONERROR | MB_SETFOREGROUND | MB_TOPMOST
    );

    return EXCEPTION_EXECUTE_HANDLER;
}

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    winrt::init_apartment(winrt::apartment_type::single_threaded);

    // test
    Services::ComRegistrationService::RegisterDynamicObject(
        __uuidof(AI::Actions::implementation::ProcessesActionProvider),
        winrt::make_self<AI::Actions::factory_implementation::ProcessesActionProviderFactory>()
    );

    SetUnhandledExceptionFilter(PlatformExceptionFilter);

#ifdef DEBUG_E
    RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, nullptr);
#endif 

    winrt::Microsoft::UI::Xaml::Application::Start([](auto&&)
    {
        winrt::make<winrt::SystemExplorer::implementation::App>();
    });

    return 0;
}