#pragma once
#include "../eil/settings.h"

namespace winrt
{
    using namespace winrt::Microsoft::UI::Xaml;
}

namespace winrt::SystemExplorer::Core::Settings
{
    struct AdvancedSettings : eil::settings_base
    {
        eil::single_threaded_rw_setting<bool, L"AdvancedAiAgent"> AdvancedAiAgent{ false };
        eil::single_threaded_rw_setting<bool, L"ProcessScaningModel"> ProcessScaningModel{ false };
        eil::single_threaded_rw_setting<bool, L"InStartup"> InStartup{ false };
    };

    struct AppearanceSettings : eil::settings_base
    {
        eil::single_threaded_rw_setting<Data::Enums::BackdropMaterialType, L"BackdropMaterial"> BackdropMaterial{ Data::Enums::BackdropMaterialType::Mica };
        eil::single_threaded_rw_setting<Data::Enums::Stretch, L"AppThemeBackgroundImageFit"> AppThemeBackgroundImageFit{ Data::Enums::Stretch::None };
        eil::single_threaded_rw_setting<winrt::hstring, L"ApplicationBackgroundColor"> ApplicationBackgroundColor{ L"#00000000" };
        eil::single_threaded_rw_setting<winrt::ElementTheme, L"ApplicationTheme"> ApplicationTheme{ winrt::ElementTheme::Light };
        eil::single_threaded_rw_setting<Data::Enums::VerticalAlignment, L"AppThemeBackgroundImageVerticalAlignment"> AppThemeBackgroundImageVerticalAlignment{ Data::Enums::VerticalAlignment::Center };
        eil::single_threaded_rw_setting<Data::Enums::HorizontalAlignment, L"AppThemeBackgroundImageHorizontalAlignment"> AppThemeBackgroundImageHorizontalAlignment{ Data::Enums::HorizontalAlignment::Center };
        eil::single_threaded_rw_setting<winrt::hstring, L"AppThemeBackgroundImageSource"> AppThemeBackgroundImageSource{ L"" };
        eil::single_threaded_rw_setting<float, L"AppThemeBackgroundImageOpacity"> AppThemeBackgroundImageOpacity{ 0.5f };
        eil::single_threaded_rw_setting<int32_t, L"ItemFontSize"> ItemFontSize{ 13 };
        eil::single_threaded_rw_setting<int32_t, L"ItemIconSize"> ItemIconSize{ 18 };
    };

    struct GeneralSettings : eil::settings_base
    {
        eil::single_threaded_rw_setting<uint16_t, L"RealTimeUpdateSpeedMs"> RealTimeUpdateSpeedMs{ 1500 };
        eil::single_threaded_rw_setting<winrt::hstring, L"StartPage"> StartPage{ L"SystemExplorer.Views.Pages.Activites.ProcessesPage" };
        eil::single_threaded_rw_setting<bool, L"MemoryDumpCaptureHypervisorPages"> MemoryDumpCaptureHypervisorPages{ false };
        eil::single_threaded_rw_setting<bool, L"MemoryDumpAbortIfInsufficientMemory"> MemoryDumpAbortIfInsufficientMemory{ false };
        eil::single_threaded_rw_setting<bool, L"MemoryDumpIncludeNonessentialPages"> MemoryDumpIncludeNonessentialPages{ false };
        eil::single_threaded_rw_setting<bool, L"MemoryDumpCaptureUserPages"> MemoryDumpCaptureUserPages{ false };
        eil::single_threaded_rw_setting<bool, L"AppWindowAlwaysOnTop"> AppWindowAlwaysOnTop{ false };
        eil::single_threaded_rw_setting<bool, L"AppWindowMinimizeOnUse"> AppWindowMinimizeOnUse{ false };
        eil::single_threaded_rw_setting<bool, L"AppWindowHideWhenMinimized"> AppWindowHideWhenMinimized{ false };
    };

    struct UserSettings
    {
        static inline wil::single_threaded_property<AdvancedSettings> AdvancedSettings;
        static inline wil::single_threaded_property<AppearanceSettings> AppearanceSettings;
        static inline wil::single_threaded_property<GeneralSettings> GeneralSettings;
    };

    struct InternalSettings : eil::settings_base
    {
        static inline eil::single_threaded_rw_setting<int32_t, L"ProcessesNameColumnLastWidth"> ProcessesNameColumnLastWidth{ 250 };
        static inline eil::single_threaded_rw_setting<int32_t, L"ProcessesStatusColumnLastWidth"> ProcessesStatusColumnLastWidth{ 50 };
        static inline eil::single_threaded_rw_setting<int32_t, L"ProcessesPidColumnLastWidth"> ProcessesPidColumnLastWidth{ 50 };
        static inline eil::single_threaded_rw_setting<int32_t, L"ProcessesCpuUsageColumnLastWidth"> ProcessesCpuUsageColumnLastWidth{ 75 };
        static inline eil::single_threaded_rw_setting<int32_t, L"ProcessesIoRateColumnLastWidth"> ProcessesIoRateColumnLastWidth{ 75 };
        static inline eil::single_threaded_rw_setting<int32_t, L"ProcessesPrivateBytesLastWidth"> ProcessesPrivateBytesLastWidth{ 50 };
    };
}