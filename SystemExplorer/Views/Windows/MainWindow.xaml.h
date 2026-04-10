#pragma once
#include "Views/Windows/MainWindow.g.h"

#include <winrt/Microsoft.UI.Windowing.h>
#include <Core/Settings/UserSettings.h>
#include <Core/Services/AppResourcesService.h>

namespace winrt::SystemExplorer::Views::Windows::implementation
{
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Microsoft::UI::Windowing;

    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow()
        {
            InitializeComponent();
            ExtendsContentIntoTitleBar(true);
            AppWindow().TitleBar().PreferredHeightOption(TitleBarHeightOption::Tall);
            
            Core::Settings::UserSettings::Instance().AppearanceSettings().SettingChanged([this](auto& sender, auto& args) {
                this->OnSystemBackdropSettingChanged(sender, args);
                this->OnApplicationThemeSettingChanged(sender, args);
            });

            LoadSettings();
        }
        void LoadSettings();
        void OnSystemBackdropSettingChanged(IInspectable const& sender, Core::Data::EventArguments::SettingChangedEventArgs const& args);
        void OnApplicationThemeSettingChanged(IInspectable const& sender, Core::Data::EventArguments::SettingChangedEventArgs const& args);
    };
}

namespace winrt::SystemExplorer::Views::Windows::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
