#pragma once

#include "Core/Settings/AppearanceSettings.g.h"
#include <Core/Serialization/SettingsBase.h>

namespace winrt::SystemExplorer::Core::Settings::implementation
{
    using namespace winrt::Microsoft::UI::Xaml::Media;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Windows::UI;

    using namespace winrt::XamlToolkit::WinUI;

    struct AppearanceSettings : AppearanceSettingsT<AppearanceSettings, Serialization::implementation::SettingsBase>
    {
        AppearanceSettings() = default;

        [[nodiscard]] Core::Data::Enums::BackdropMaterialType BackdropMaterial() { return Get(L"BackdropMaterial", Core::Data::Enums::BackdropMaterialType::Mica); }
        void BackdropMaterial(Core::Data::Enums::BackdropMaterialType value) { Set(L"BackdropMaterial", value); }

        [[nodiscard]] ElementTheme ApplicationTheme() { return Get(L"ApplicationTheme", ElementTheme::Light); }
        void ApplicationTheme(ElementTheme value) { Set(L"ApplicationTheme", value); }

        [[nodiscard]] hstring AppThemeBackgroundColor() { return  Get(L"AppThemeBackgroundColor", hstring{ L"#00000000" }); }
        void AppThemeBackgroundColor(hstring const& value) { Set(L"AppThemeBackgroundColor", value); }
    };
}

namespace winrt::SystemExplorer::Core::Settings::factory_implementation
{
    struct AppearanceSettings : AppearanceSettingsT<AppearanceSettings, implementation::AppearanceSettings>
    {
    };
}
