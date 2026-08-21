#include "pch.h"
#include "winrt_module_imports.h"
#include "AppearanceViewModel.h"

#include <Core/Settings/Settings.h>
#include <Core/Services/AppResourcesService.h>

#include <Helpers/EnumHelper.h>

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    void AppearanceViewModel::SelectedAppThemeIndex(int32_t value) noexcept
    {
        static constexpr std::array themes = { ElementTheme::Light, ElementTheme::Dark, ElementTheme::Default };
        if (value >= 0 && value < static_cast<int32_t>(themes.size()))
        {
            SelectedAppThemeIndex_ = value;
            Core::Settings::UserSettings::AppearanceSettings.ApplicationTheme(themes[value]);
            RaisePropertyChanged(L"SelectedAppThemeIndex");
        }
    }

    void AppearanceViewModel::SelectedAppThemeResources(AppThemeResourceItem value) noexcept
    {
        if (value != SelectedAppThemeResources_)
        {
            SelectedAppThemeResources_ = std::move(value);
            AppThemeBackgroundColor(SelectedAppThemeResources_.BackgroundColor());
            RaisePropertyChanged(L"SelectedAppThemeResources");
        }
    }

    hstring AppearanceViewModel::AppThemeBackgroundColor() const noexcept
    {
        return Core::Settings::UserSettings::AppearanceSettings.ApplicationBackgroundColor();
    }

    void AppearanceViewModel::AppThemeBackgroundColor(hstring value) noexcept
    {
        Core::Services::AppResourcesService::SetAppThemeBackgroundColor(winrt::XamlToolkit::WinUI::Helpers::ColorHelper::ToColor(std::move(value))); 
        Core::Settings::UserSettings::AppearanceSettings.ApplicationBackgroundColor(value);
    }

    void AppearanceViewModel::SelectedBackdropMaterial(IInspectable value) noexcept
    {
        if (SelectedBackdropMaterial_ != value)
        {
            SelectedBackdropMaterial_ = std::move(value);
            Core::Settings::UserSettings::AppearanceSettings.BackdropMaterial(Helpers::EnumHelper::Map<Data::Enums::BackdropMaterialType>(unbox_value<hstring>(SelectedBackdropMaterial_)));
            RaisePropertyChanged(L"SelectedBackdropMaterial");
        }
    }

    hstring AppearanceViewModel::AppThemeBackgroundImageSource() const noexcept
    {
        return Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageSource();
    }

    void AppearanceViewModel::AppThemeBackgroundImageSource(hstring value) noexcept
    {
        AppThemeBackgroundImageSource_ = std::move(value);
        Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageSource(AppThemeBackgroundImageSource_);
        RaisePropertyChanged(L"AppThemeBackgroundImageSource");
    }

    float AppearanceViewModel::AppThemeBackgroundImageOpacity() const noexcept
    {
        return Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageOpacity();
    }

    void AppearanceViewModel::AppThemeBackgroundImageOpacity(float value) noexcept
    {
        Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageOpacity(value);
        RaisePropertyChanged(L"AppThemeBackgroundImageOpacity");
    }

    void AppearanceViewModel::SelectedImageStretchType(IInspectable value) noexcept
    {
        Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageFit(Helpers::EnumHelper::Map<Data::Enums::Stretch>(unbox_value<hstring>(std::move(value))));
    }

    void AppearanceViewModel::SelectedImageVerticalAlignmentType(IInspectable value) noexcept
    {
        Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageVerticalAlignment(Helpers::EnumHelper::Map<Data::Enums::VerticalAlignment>(unbox_value<hstring>(std::move(value))));
    }

    void AppearanceViewModel::SelectedImageHorizontalAlignmentType(IInspectable value) noexcept
    {
        Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageHorizontalAlignment(Helpers::EnumHelper::Map<Data::Enums::HorizontalAlignment>(unbox_value<hstring>(std::move(value))));
    }
}