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

        [[nodiscard]] int32_t ProcessesNameColumnLastWidth() { return Get(L"ProcessesNameColumnLastWidth", int32_t{ 250 }); }
        void ProcessesNameColumnLastWidth(int32_t value) { Set(L"ProcessesNameColumnLastWidth", value); }

        [[nodiscard]] int32_t ProcessesStatusColumnLastWidth() { return Get(L"ProcessesStatusColumnLastWidth", int32_t{ 50 }); }
        void ProcessesStatusColumnLastWidth(int32_t value) { Set(L"ProcessesStatusColumnLastWidth", value); }

        [[nodiscard]] int32_t ProcessesPidColumnLastWidth() { return Get(L"ProcessesPidColumnLastWidth", int32_t{ 50 }); }
        void ProcessesPidColumnLastWidth(int32_t value) { Set(L"ProcessesPidColumnLastWidth", value); }

        [[nodiscard]] int32_t ProcessesCpuUsageColumnLastWidth() { return Get(L"ProcessesCpuUsageColumnLastWidth", int32_t{ 75 }); }
        void ProcessesCpuUsageColumnLastWidth(int32_t value) { Set(L"ProcessesCpuUsageColumnLastWidth", value); }

        [[nodiscard]] int32_t ProcessesIoRateColumnLastWidth() { return Get(L"ProcessesIoRateColumnLastWidth", int32_t{ 75 }); }
        void ProcessesIoRateColumnLastWidth(int32_t value) { Set(L"ProcessesIoRateColumnLastWidth", value); }

        [[nodiscard]] int32_t ProcessesPrivateBytesLastWidth() { return Get(L"ProcessesPrivateBytesLastWidth", int32_t{ 50 }); }
        void ProcessesPrivateBytesLastWidth(int32_t value) { Set(L"ProcessesPrivateBytesLastWidth", value); }

        [[nodiscard]] int32_t ItemFontSize() { return Get(L"ItemFontSize", int32_t{ 13 }); }
        void ItemFontSize(int32_t value) { Set(L"ItemFontSize", value); }

        [[nodiscard]] int32_t ItemIconSize() { return Get(L"ItemIconSize", int32_t{ 18 }); }
        void ItemIconSize(int32_t value) { Set(L"ItemIconSize", value); }
    };
}
FACTORY(winrt::SystemExplorer::Core::Settings, AppearanceSettings);