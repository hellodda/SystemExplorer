#include "pch.h"
#include "App.xaml.h"
#include "Helpers/Win32Helper.h"

LONG WINAPI PlatformExceptionFilter(
    _In_ PEXCEPTION_POINTERS pExceptionInfo
)
{
    const auto record = pExceptionInfo->ExceptionRecord;

    std::wstring errorDesc = winrt::SystemExplorer::Helpers::Win32Helper::GetErrorMessage(record->ExceptionCode);

    std::wstring message = std::format(
        L"A critical application error has occurred.\n\n"
        L"Exception Details:\n"
        L"Faulting Code: 0x{:08X}\n"
        L"Description: {}\n"
        L"Instruction Address: 0x{:p}\n",
        record->ExceptionCode,
        errorDesc,
        record->ExceptionAddress
    );

    MessageBoxW(
        NULL,
        message.c_str(),
        L"System Explorer - Critical Error",
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

    SetUnhandledExceptionFilter(PlatformExceptionFilter);

    winrt::Microsoft::UI::Xaml::Application::Start([](auto&&)
    {
        winrt::make<winrt::SystemExplorer::implementation::App>();
    });

    return 0;
}