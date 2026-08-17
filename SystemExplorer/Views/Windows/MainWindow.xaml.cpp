#include "pch.h"
#include "winrt_module_imports.h"
#include "MainWindow.xaml.h"
#if __has_include("Views/Windows/MainWindow.g.cpp")
#include "Views/Windows/MainWindow.g.cpp"
#endif
#include <Core/Settings/Settings.h>

namespace winrt::SystemExplorer::Views::Windows::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();

        ExtendsContentIntoTitleBar(true);
        AppWindow().TitleBar().PreferredHeightOption(winrt::TitleBarHeightOption::Tall);

        winrt::SystemExplorer::Core::Settings::UserSettings::AppearanceSettings.SettingChanged->operator()([weak = get_weak()](std::wstring_view name, winrt::IInspectable const& value)
        {
            if (auto wrf = weak.get())
            {
                if (name == L"ApplicationTheme")
                {
                    auto theme = value.as<winrt::ElementTheme>();

                    wrf->RootGrid().RequestedTheme(theme);
                }
            }
        });
    }
}

