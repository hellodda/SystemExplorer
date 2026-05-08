#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("Views/Windows/MainWindow.g.cpp")
#include "Views/Windows/MainWindow.g.cpp"
#endif
#include <Core/Services/AppResourcesService.h>
#include <Helpers/EnumHelper.h>
#include <Helpers/UI/AppSystemBackdrop.h>

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::WinUI3Package;

namespace winrt::SystemExplorer::Views::Windows::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();

        ExtendsContentIntoTitleBar(true);
        AppWindow().TitleBar().PreferredHeightOption(TitleBarHeightOption::Tall);

        Core::Settings::UserSettings::Instance().AppearanceSettings().SettingChanged([this](auto& sender, auto& args) {
            this->OnApplicationThemeSettingChanged(sender, args);
        });

        LoadSettings();
    }


    void MainWindow::LoadSettings()
    {
        auto appearanceSettings = Core::Settings::UserSettings::Instance().AppearanceSettings();

        this->SystemBackdrop(Helpers::UI::AppSystemBackdrop{});
        this->RootGrid().RequestedTheme(appearanceSettings.ApplicationTheme());

        winrt::SystemExplorer::Core::Services::AppResourcesService::Instance().SetAppThemeBackgroundColor(XamlToolkit::WinUI::Helpers::ColorHelper::ColorHelper::ToColor(appearanceSettings.AppThemeBackgroundColor()));
    }
    void MainWindow::OnApplicationThemeSettingChanged(IInspectable const& sender, Core::Data::EventArguments::SettingChangedEventArgs const& args)
    {
        if (args.SettingName() == L"ApplicationTheme")
        {
            this->RootGrid().RequestedTheme(unbox_value<ElementTheme>(args.NewValue()));
        }
    }
}

