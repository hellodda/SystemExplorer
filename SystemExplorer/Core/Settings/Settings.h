#pragma once
#include <EIL/settings.h>

#ifdef __INTELLISENSE__
#include <wil/cppwinrt_authoring.h>
#include <winrt/SystemExplorer.Core.Data.Enums.h>
#endif

namespace winrt
{
    using namespace winrt::Microsoft::UI::Xaml;
}

namespace winrt::SystemExplorer::Core::Settings
{
    struct AdvancedSettings : eil::settings_base<AdvancedSettings>
    {
        eil::single_threaded_rw_setting<bool, "AdvancedAiAgent"> AdvancedAiAgent{ false };
        eil::single_threaded_rw_setting<bool, "ProcessScaningModel"> ProcessScaningModel{ false };
        eil::single_threaded_rw_setting<bool, "InStartup"> InStartup{ false };
    };

    struct AppearanceSettings : eil::settings_base<AppearanceSettings>
    {
        eil::single_threaded_rw_setting<Data::Enums::BackdropMaterialType, "BackdropMaterial"> BackdropMaterial{ Data::Enums::BackdropMaterialType::Mica };
        eil::single_threaded_rw_setting<Data::Enums::Stretch, "AppThemeBackgroundImageFit"> AppThemeBackgroundImageFit{ Data::Enums::Stretch::None };
        eil::single_threaded_rw_setting<winrt::hstring, "ApplicationBackgroundColor"> ApplicationBackgroundColor{ L"#00000000" };
        eil::single_threaded_rw_setting<winrt::ElementTheme, "ApplicationTheme"> ApplicationTheme{ winrt::ElementTheme::Light };
        eil::single_threaded_rw_setting<Data::Enums::VerticalAlignment, "AppThemeBackgroundImageVerticalAlignment"> AppThemeBackgroundImageVerticalAlignment{ Data::Enums::VerticalAlignment::Center };
        eil::single_threaded_rw_setting<Data::Enums::HorizontalAlignment, "AppThemeBackgroundImageHorizontalAlignment"> AppThemeBackgroundImageHorizontalAlignment{ Data::Enums::HorizontalAlignment::Center };
        eil::single_threaded_rw_setting<winrt::hstring, "AppThemeBackgroundImageSource"> AppThemeBackgroundImageSource{ L"" };
        eil::single_threaded_rw_setting<float, "AppThemeBackgroundImageOpacity"> AppThemeBackgroundImageOpacity{ 0.5f };
        eil::single_threaded_rw_setting<int32_t, "ItemFontSize"> ItemFontSize{ 13 };
        eil::single_threaded_rw_setting<int32_t, "ItemIconSize"> ItemIconSize{ 18 };
    };

    struct GeneralSettings : eil::settings_base<GeneralSettings>
    {
        eil::single_threaded_rw_setting<uint16_t, "RealTimeUpdateSpeedMs"> RealTimeUpdateSpeedMs{ 1500 };
        eil::single_threaded_rw_setting<winrt::hstring, "StartPage"> StartPage{ L"ProcessesPage" };
        eil::single_threaded_rw_setting<bool, "MemoryDumpCaptureHypervisorPages"> MemoryDumpCaptureHypervisorPages{ false };
        eil::single_threaded_rw_setting<bool, "MemoryDumpAbortIfInsufficientMemory"> MemoryDumpAbortIfInsufficientMemory{ false };
        eil::single_threaded_rw_setting<bool, "MemoryDumpIncludeNonessentialPages"> MemoryDumpIncludeNonessentialPages{ false };
        eil::single_threaded_rw_setting<bool, "MemoryDumpCaptureUserPages"> MemoryDumpCaptureUserPages{ false };
        eil::single_threaded_rw_setting<bool, "AppWindowAlwaysOnTop"> AppWindowAlwaysOnTop{ false };
        eil::single_threaded_rw_setting<bool, "AppWindowMinimizeOnUse"> AppWindowMinimizeOnUse{ false };
        eil::single_threaded_rw_setting<bool, "AppWindowHideWhenMinimized"> AppWindowHideWhenMinimized{ false };
    };

    struct UserSettings
    {
        static inline wil::single_threaded_property<AdvancedSettings> AdvancedSettings;
        static inline wil::single_threaded_property<AppearanceSettings> AppearanceSettings;
        static inline wil::single_threaded_property<GeneralSettings> GeneralSettings;
    };

    struct InternalSettings : eil::settings_base<GeneralSettings>
    {
        static inline eil::single_threaded_rw_setting<int32_t, "ProcessesNameColumnLastWidth"> ProcessesNameColumnLastWidth{ 250 };
        static inline eil::single_threaded_rw_setting<int32_t, "ProcessesStatusColumnLastWidth"> ProcessesStatusColumnLastWidth{ 50 };
        static inline eil::single_threaded_rw_setting<int32_t, "ProcessesPidColumnLastWidth"> ProcessesPidColumnLastWidth{ 50 };
        static inline eil::single_threaded_rw_setting<int32_t, "ProcessesCpuUsageColumnLastWidth"> ProcessesCpuUsageColumnLastWidth{ 75 };
        static inline eil::single_threaded_rw_setting<int32_t, "ProcessesIoRateColumnLastWidth"> ProcessesIoRateColumnLastWidth{ 75 };
        static inline eil::single_threaded_rw_setting<int32_t, "ProcessesPrivateBytesLastWidth"> ProcessesPrivateBytesLastWidth{ 50 };
    };
}