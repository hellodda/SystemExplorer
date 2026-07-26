#include "pch.h"
#include "winrt_module_imports.h"
#include "AppSystemBackdrop.h"
#if __has_include("Helpers/UI/AppSystemBackdrop.g.cpp")
#include "Helpers/UI/AppSystemBackdrop.g.cpp"
#endif
#include <Core/Settings/Settings.h>


namespace winrt::SystemExplorer::Helpers::UI::implementation
{
	AppSystemBackdrop::AppSystemBackdrop()
	{
      /*  Core::Settings::UserSettings::AppearanceSettings().SettingChanged([weak = get_weak()](std::string_view name, eil::generic_t) {
            if (auto wrf = weak.get())
            {
                if (name == "BackdropMaterial")
                {

                }
            }
        });*/
	}

	AppSystemBackdrop::AppSystemBackdrop(bool enableWhenInactive)
	{
        EnableWhenInactive(enableWhenInactive);

       /* Core::Settings::UserSettings::AppearanceSettings().SettingChanged([weak = get_weak()](std::string_view name, eil::generic_t) {
            if (auto wrf = weak.get())
            {
                if (name == "BackdropMaterial")
                {

                }
            }
        });*/
	}

    void AppSystemBackdrop::EnableWhenInactive(bool value) noexcept
    {
        SetValue(EnableWhenInactiveProperty, winrt::box_value(value));
    }

    bool AppSystemBackdrop::EnableWhenInactive() const noexcept
    {
        return winrt::unbox_value<bool>(GetValue(EnableWhenInactiveProperty));
    }

    void AppSystemBackdrop::OnTargetConnected(winrt::ICompositionSupportsSystemBackdrop const& connectedTarget, XamlRoot const& xamlRoot)
    {
        if (target_)
            throw hresult_invalid_argument(L"AppSystemBackdrop cannot be used with more than one target");

        base_type::OnTargetConnected(connectedTarget, xamlRoot);

        target_ = connectedTarget;
        root_ = xamlRoot;

        auto configuration = GetDefaultSystemBackdropConfiguration(connectedTarget, xamlRoot);
        configuration.IsInputActive(EnableWhenInactive());

        controller_ = getSystemBackdropController(Core::Settings::UserSettings::AppearanceSettings().BackdropMaterial(), configuration.Theme());
        controller_.SetSystemBackdropConfiguration(configuration);
        controller_.AddSystemBackdropTarget(connectedTarget);
    }

    void AppSystemBackdrop::OnTargetDisconnected(winrt::ICompositionSupportsSystemBackdrop const& connectedTarget)
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
    void AppSystemBackdrop::OnDefaultSystemBackdropConfigurationChanged(winrt::ICompositionSupportsSystemBackdrop const& target, winrt::XamlRoot const& xamlRoot)
    {
        base_type::OnDefaultSystemBackdropConfigurationChanged(target, xamlRoot);

        auto configuration = GetDefaultSystemBackdropConfiguration(target, xamlRoot);
        configuration.IsInputActive(EnableWhenInactive());

        if (auto acrylicController = controller_.try_as<winrt::DesktopAcrylicController>())
        {
            if (acrylicController.Kind() != DesktopAcrylicKind::Thin || configuration.Theme() == prevTheme_)
                return;

            prevTheme_ = configuration.Theme();
            setThinAcrylicBackdropProperties(acrylicController, configuration.Theme());
        }
    }

	void AppSystemBackdrop::onSettingChanged()
	{
        if (!target_)
            return;

        if (controller_)
        {
            controller_.RemoveAllSystemBackdropTargets();
            controller_.Close();
        }

        auto configuration = GetDefaultSystemBackdropConfiguration(target_, root_);
        configuration.IsInputActive(EnableWhenInactive());

        controller_ = getSystemBackdropController(Core::Settings::UserSettings::AppearanceSettings().BackdropMaterial(), configuration.Theme());
            
        if (controller_)
        {
            controller_.SetSystemBackdropConfiguration(configuration);
            controller_.AddSystemBackdropTarget(target_);
        }
	}

    void AppSystemBackdrop::setThinAcrylicBackdropProperties(winrt::DesktopAcrylicController const& controller, winrt::SystemBackdropTheme theme)
    {
        switch (theme)
        {
        case SystemBackdropTheme::Light:
        {
            controller.TintColor(Windows::UI::ColorHelper::FromArgb(0xff, 0xd3, 0xd3, 0xd3));
            controller.FallbackColor(Windows::UI::ColorHelper::FromArgb(0x99, 0xd3, 0xd3, 0xd3));
            controller.TintOpacity(0.0f);
            controller.LuminosityOpacity(0.44f);
            break;
        }
        case SystemBackdropTheme::Dark:
        {
            controller.TintColor(Windows::UI::ColorHelper::FromArgb(0xff, 0x54, 0x54, 0x54));
            controller.FallbackColor(Windows::UI::ColorHelper::FromArgb(0xff, 0x20, 0x20, 0x20));
            controller.TintOpacity(0.0f);
            controller.LuminosityOpacity(0.64f);
            break;
        }
        }
    }

    ISystemBackdropControllerWithTargets AppSystemBackdrop::getSystemBackdropController(Core::Data::Enums::BackdropMaterialType backdropType, winrt::SystemBackdropTheme theme)
    {
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
