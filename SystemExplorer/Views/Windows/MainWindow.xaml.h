#pragma once
#include "Views/Windows/MainWindow.g.h"
#include <winrt/Microsoft.UI.Windowing.h>
#include <Core/Settings/UserSettings.h>
#include <Core/Services/AppResourcesService.h>
#include <factory.h>

namespace winrt::SystemExplorer::Views::Windows::implementation
{
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Microsoft::UI::Windowing;

    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();
        void LoadSettings();
        void OnSystemBackdropSettingChanged(IInspectable const& sender, Core::Data::EventArguments::SettingChangedEventArgs const& args);
        void OnApplicationThemeSettingChanged(IInspectable const& sender, Core::Data::EventArguments::SettingChangedEventArgs const& args);
    };
}
FACTORY(winrt::SystemExplorer::Views::Windows, MainWindow);

