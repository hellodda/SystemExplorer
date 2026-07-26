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

      /*  Core::Settings::UserSettings::AppearanceSettings.SettingChanged([this](std::string_view name, eil::generic_t value)
        {
            
            if (name == "ApplicationTheme")
            {
                auto theme = value.as<winrt::ElementTheme>();

                this->RootGrid().RequestedTheme(theme);
            }

        });*/
    }
}

