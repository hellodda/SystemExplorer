#include "pch.h"
#include "winrt_module_imports.h"
#include "App.xaml.h"
#include "Views/Windows/MainWindow.xaml.h"

namespace winrt::SystemExplorer::implementation
{
    App::App()
    {
#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException([](winrt::IInspectable const&, winrt::UnhandledExceptionEventArgs const& e)
        {
            if (IsDebuggerPresent())
            {
                auto errorMessage = e.Message();
                __debugbreak();
            }
        });
#endif
    }

    void App::OnLaunched([[maybe_unused]] winrt::LaunchActivatedEventArgs const& e)
    {
        window_ = make<Views::Windows::implementation::MainWindow>();
        window_.Activate();
    }

    winrt::WindowId App::GetCurrentWindowId() noexcept
    {
        return window_.AppWindow().Id();
    }
}
