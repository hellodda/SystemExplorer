#include "pch.h"
#include "AppSystemBackdrop.h"
#if __has_include("Helpers/UI/AppSystemBackdrop.g.cpp")
#include "Helpers/UI/AppSystemBackdrop.g.cpp"
#endif
#include <Core/Settings/UserSettings.h>
#include <winrt/Windows.UI.h>

using namespace winrt::SystemExplorer::Core::Settings;
using namespace winrt::SystemExplorer::Core::Data::Enums;

using namespace winrt::WinUI3Package;

namespace winrt::SystemExplorer::Helpers::UI::implementation
{
	AppSystemBackdrop::AppSystemBackdrop()
	{
        UserSettings::Instance().AppearanceSettings().SettingChanged([weak = get_weak()](auto& s, auto& a) {
            if (auto wrf = weak.get())
            {
                wrf->onSettingChanged(s, a);
            }
        });
	}
	AppSystemBackdrop::AppSystemBackdrop(bool enableWhenInactive)
	{
        isSecondaryWindow_ = enableWhenInactive;

        UserSettings::Instance().AppearanceSettings().SettingChanged([weak = get_weak()](auto& s, auto& a) {
            if (auto wrf = weak.get())
            {
                wrf->onSettingChanged(s, a);
            }
        });
	}
    void AppSystemBackdrop::OnTargetConnected(ICompositionSupportsSystemBackdrop const& connectedTarget, XamlRoot const& xamlRoot)
    {
        if (target_)
            throw hresult_invalid_argument(L"AppSystemBackdrop cannot be used with more than one target");

        base_type::OnTargetConnected(connectedTarget, xamlRoot);

        target_ = connectedTarget;
        root_ = xamlRoot;

        auto configuration = GetDefaultSystemBackdropConfiguration(connectedTarget, xamlRoot);
        controller_ = getSystemBackdropController(UserSettings::Instance().AppearanceSettings().BackdropMaterial(), configuration.Theme());
        controller_.SetSystemBackdropConfiguration(configuration);
        controller_.AddSystemBackdropTarget(connectedTarget);
    }
    void AppSystemBackdrop::OnTargetDisconnected(winrt::Microsoft::UI::Composition::ICompositionSupportsSystemBackdrop const& connectedTarget)
    {
        base_type::OnTargetDisconnected(connectedTarget);
        target_ = nullptr;
        root_ = nullptr;

        try
        {
            controller_.RemoveSystemBackdropTarget(connectedTarget);
        }
        catch (hresult_error const&)
        {

        }
    }
    void AppSystemBackdrop::OnDefaultSystemBackdropConfigurationChanged(ICompositionSupportsSystemBackdrop const& target, XamlRoot const& xamlRoot)
    {
        base_type::OnDefaultSystemBackdropConfigurationChanged(target, xamlRoot);

        auto configuration = GetDefaultSystemBackdropConfiguration(target, xamlRoot);

        if (auto acrylicController = controller_.try_as<DesktopAcrylicController>())
        {
            if (acrylicController.Kind() != DesktopAcrylicKind::Thin || configuration.Theme() == prevTheme_)
                return;

            prevTheme_ = configuration.Theme();
            setThinAcrylicBackdropProperties(acrylicController, configuration.Theme());
        }
    }
	void AppSystemBackdrop::onSettingChanged(IInspectable const& sender, SettingChangedEventArgs const& args)
	{
        if (!target_)
            return;


        if (args.SettingName() == L"BackdropMaterial")
        {
            controller_.RemoveAllSystemBackdropTargets();
            controller_.Close();

            auto configuration = GetDefaultSystemBackdropConfiguration(target_, root_);
            auto newController = getSystemBackdropController(UserSettings::Instance().AppearanceSettings().BackdropMaterial(), configuration.Theme());
            newController.SetSystemBackdropConfiguration(configuration);
            newController.AddSystemBackdropTarget(target_);

            controller_ = newController;
        }
	}
    void AppSystemBackdrop::setThinAcrylicBackdropProperties(DesktopAcrylicController const& controller, SystemBackdropTheme theme)
    {
        switch (theme)
        {
        case SystemBackdropTheme::Light:
        {
            controller.TintColor(Windows::UI::ColorHelper::FromArgb(0xff, 0xd3, 0xd3, 0xd3));
            controller.FallbackColor(Windows::UI::ColorHelper::FromArgb(0x99, 0xd3, 0xd3, 0xd3));
            controller.TintOpacity(0.0f);
            controller.LuminosityOpacity(0.44f);
        }
        case SystemBackdropTheme::Dark:
        {
            controller.TintColor(Windows::UI::ColorHelper::FromArgb(0xff, 0x54, 0x54, 0x54));
            controller.FallbackColor(Windows::UI::ColorHelper::FromArgb(0xff, 0x20, 0x20, 0x20));
            controller.TintOpacity(0.0f);
            controller.LuminosityOpacity(0.64f);
        }
        }
    }
    ISystemBackdropControllerWithTargets AppSystemBackdrop::getSystemBackdropController(BackdropMaterialType backdropType, SystemBackdropTheme theme)
    {
        if (isSecondaryWindow_ && backdropType == BackdropMaterialType::MicaAlt)
            backdropType = BackdropMaterialType::Mica;

        switch (backdropType)
        {   
        case winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::Mica:
        {
            auto controller = MicaController{};
            controller.Kind(MicaKind::Base);
            return controller;
        }
        case winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::MicaAlt:
        {
            auto controller = MicaController{};
            controller.Kind(MicaKind::BaseAlt);
            return controller;
        }
        case winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::Acrylic:
        {
            auto controller = DesktopAcrylicController{};
            controller.Kind(DesktopAcrylicKind::Base);
            return controller;
        }
        case winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::ThinAcrylic:
        {
            auto controller = DesktopAcrylicController{};
            controller.Kind(DesktopAcrylicKind::Thin);

            setThinAcrylicBackdropProperties(controller, theme);
            return controller;
        }
        case winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::Static:
        default:
            return nullptr;
        }
    }
}
