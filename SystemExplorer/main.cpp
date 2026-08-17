#include "pch.h"
#include "winrt_module_imports.h"
#include "App.xaml.h"

#include <winrt/Microsoft.UI.Xaml.Settings.h>
#include <core/services/AppMemoryManager.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

INT APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ INT nCmdShow
)
{
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Settings;
	using namespace winrt::SystemExplorer::Core::Services;

    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    //AppMemoryManager::LoadMemoryOptions();

  /*  InitializeLogger();
    try
    {
        InitializeCommonControls();
        InitializeExceptionPolicy();
        EnablePrivileges();
        StartEmsServer();
    }
    CATCH_LOG()*/


    XamlOptionalChanges::EnableChange(XamlChangeId::DefaultStyleOptimizations);
    XamlOptionalChanges::EnableChange(XamlChangeId::DeferContextFlyoutInit);
    XamlOptionalChanges::EnableChange(XamlChangeId::IconNoGridOptimization);
    XamlOptionalChanges::EnableChange(XamlChangeId::OptimizeApplyStyles);
    
    Application::Start([](auto&&)
    {
        winrt::make<winrt::SystemExplorer::implementation::App>();
    });

    return EXIT_SUCCESS;
}
