#pragma once

#include "Core/Settings/AppearanceSettings.g.h"
#include <Core/Serialization/SettingsBase.h>

namespace winrt::SystemExplorer::Core::Settings::implementation
{
    using namespace winrt::Microsoft::UI::Xaml::Media;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Windows::UI;

    using namespace winrt::XamlToolkit::WinUI;
    using namespace winrt::SystemExplorer::Core;

    struct AppearanceSettings : AppearanceSettingsT<AppearanceSettings, Serialization::implementation::SettingsBase>
    {
        AppearanceSettings() = default;

        [[nodiscard]] Data::Enums::BackdropMaterialType BackdropMaterial() { return Get(L"BackdropMaterial", Data::Enums::BackdropMaterialType::Mica); }
        void BackdropMaterial(Data::Enums::BackdropMaterialType value) { Set(L"BackdropMaterial", value); }

        [[nodiscard]] ElementTheme ApplicationTheme() { return Get(L"ApplicationTheme", ElementTheme::Light); }
        void ApplicationTheme(ElementTheme value) { Set(L"ApplicationTheme", value); }

        [[nodiscard]] hstring AppThemeBackgroundColor() { return  Get(L"AppThemeBackgroundColor", hstring{ L"#00000000" }); }
        void AppThemeBackgroundColor(hstring const& value) { Set(L"AppThemeBackgroundColor", value); }

        [[nodiscard]] Data::Enums::Stretch AppThemeBackgroundImageFit() { return Get(L"AppThemeBackgroundImageFit", Data::Enums::Stretch::None); }
        void AppThemeBackgroundImageFit(Data::Enums::Stretch value) { Set(L"AppThemeBackgroundImageFit", value); }

        [[nodiscard]] Data::Enums::VerticalAlignment AppThemeBackgroundImageVerticalAlignment() { return Get(L"AppThemeBackgroundImageVerticalAlignment", Data::Enums::VerticalAlignment::Center); }
        void AppThemeBackgroundImageVerticalAlignment(Data::Enums::VerticalAlignment value) { Set(L"AppThemeBackgroundImageVerticalAlignment", value); }

        [[nodiscard]] Data::Enums::HorizontalAlignment AppThemeBackgroundImageHorizontalAlignment() { return Get(L"AppThemeBackgroundImageHorizontalAlignment", Data::Enums::HorizontalAlignment::Center); }
        void AppThemeBackgroundImageHorizontalAlignment(Data::Enums::HorizontalAlignment value) { Set(L"AppThemeBackgroundImageHorizontalAlignment", value); }

        [[nodiscard]] hstring AppThemeBackgroundImageSource() { return Get(L"AppThemeBackgroundImageSource", hstring{ L"" }); }
        void AppThemeBackgroundImageSource(hstring const& value) { Set(L"AppThemeBackgroundImageSource", value); }

        [[nodiscard]] float AppThemeBackgroundImageOpacity() { return Get(L"AppThemeBackgroundImageOpacity", float{ 0.5 }); }
        void AppThemeBackgroundImageOpacity(float value) { Set(L"AppThemeBackgroundImageOpacity", value); }
    };
}
FACTORY(winrt::SystemExplorer::Core::Settings, AppearanceSettings);