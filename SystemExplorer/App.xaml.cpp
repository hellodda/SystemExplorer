#include "pch.h"
#include "App.xaml.h"

#if __has_include("module.g.cpp")
#  include "module.g.cpp"
#endif
#include "Views/Windows/MainWindow.xaml.h"
#include "Core/Settings/UserSettings.h"

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::SystemExplorer::implementation
{
	Window App::window_{ nullptr };

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
        window_ = make<Views::Windows::implementation::MainWindow>();
        window_.Activate();
    }
}
