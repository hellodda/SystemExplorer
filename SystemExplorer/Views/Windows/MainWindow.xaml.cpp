#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("Views/Windows/MainWindow.g.cpp")
#include "Views/Windows/MainWindow.g.cpp"
#endif
#include <Core/Services/AppResourcesService.h>
#include <Helpers/EnumHelper.h>

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;


namespace winrt::SystemExplorer::Views::Windows::implementation
{
    MainWindow::MainWindow()
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

    winrt::Microsoft::UI::Xaml::Media::SystemBackdrop GetBackdrop(Core::Data::Enums::BackdropMaterialType material)
    {
        switch (material)
        {
        case Core::Data::Enums::BackdropMaterialType::Static: return nullptr;
        case Core::Data::Enums::BackdropMaterialType::Mica: return Microsoft::UI::Xaml::Media::MicaBackdrop{};
        case Core::Data::Enums::BackdropMaterialType::MicaAlt:
        {
            auto backdrop = Microsoft::UI::Xaml::Media::MicaBackdrop{};
            backdrop.Kind(Microsoft::UI::Composition::SystemBackdrops::MicaKind::BaseAlt);

            return backdrop;
        }
        case Core::Data::Enums::BackdropMaterialType::Acrylic: return Microsoft::UI::Xaml::Media::DesktopAcrylicBackdrop{};
        case Core::Data::Enums::BackdropMaterialType::ThinAcrylic: return Microsoft::UI::Xaml::Media::DesktopAcrylicBackdrop{};
        default: return nullptr;
        }
    }


    void MainWindow::LoadSettings()
    {
        auto appearanceSettings = Core::Settings::UserSettings::Instance().AppearanceSettings();

        this->SystemBackdrop(GetBackdrop(appearanceSettings.BackdropMaterial()));
        this->RootGrid().RequestedTheme(appearanceSettings.ApplicationTheme());

        winrt::SystemExplorer::Core::Services::AppResourcesService::Instance().SetAppThemeBackgroundColor(XamlToolkit::WinUI::Helpers::ColorHelper::ColorHelper::ToColor(appearanceSettings.AppThemeBackgroundColor()));
    }
    void MainWindow::OnSystemBackdropSettingChanged(IInspectable const& sender, Core::Data::EventArguments::SettingChangedEventArgs const& args)
	{
        if (args.SettingName() == L"BackdropMaterial")
        {
            auto type = unbox_value<Core::Data::Enums::BackdropMaterialType>(args.NewValue());
            auto typeStr = SystemExplorer::Helpers::EnumHelper::Map(type);
            this->SystemBackdrop(GetBackdrop(type));
        }
    }
    void MainWindow::OnApplicationThemeSettingChanged(IInspectable const& sender, Core::Data::EventArguments::SettingChangedEventArgs const& args)
    {
        if (args.SettingName() == L"ApplicationTheme")
        {
            this->RootGrid().RequestedTheme(unbox_value<ElementTheme>(args.NewValue()));
        }
    }
}

