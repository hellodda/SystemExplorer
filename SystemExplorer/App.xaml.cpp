#include "pch.h"
#include "App.xaml.h"

#if __has_include("module.g.cpp")
#  include "module.g.cpp"
#endif
#include "Views/Windows/MainWindow.xaml.h"
#include "Core/Settings/UserSettings.h"
#include "Core/Services/AppResourcesService.h"
#include "Core/Diagnostics/AsyncFileLogger.h"
#include <Core/Serialization/functional.h>
#include <wil/result_macros.h>

#include <winrt/Windows.AI.Actions.h>

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::SystemExplorer::implementation
{
	WindowEx App::window_{ nullptr };
    AppModel App::appModel_{};

    App::App()
    {
#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException([](IInspectable const&, UnhandledExceptionEventArgs const& e)
        {
            if (IsDebuggerPresent())
            {
                auto errorMessage = e.Message();
                __debugbreak();
            }
        });
#endif
    }

    void App::OnLaunched([[maybe_unused]] LaunchActivatedEventArgs const& e)
    {
        auto appDataPath = winrt::Microsoft::Windows::Storage::ApplicationData::GetDefault().LocalCacheFolder().Path();

		Core::Diagnostics::AsyncFileLogger::Instance().Initialize(appDataPath.c_str() + std::wstring(L"\\SystemExplorer.log"));

        wil::SetResultLoggingCallback(WilResultLoggingCallBack);

        window_ = make<Views::Windows::implementation::MainWindow>();
        window_.Activate();
    }
}
