#pragma once

#include "Core/Settings/GeneralSettings.g.h"

#include <Core/Serialization/SettingsBase.h>

namespace winrt::SystemExplorer::Core::Settings::implementation
{
    struct GeneralSettings : GeneralSettingsT<GeneralSettings, Serialization::implementation::SettingsBase>
    {
        GeneralSettings() = default;

        uint16_t RealTimeUpdateSpeedMs() { return Get(L"RealTimeUpdateSpeedMs", 1500); }
        void RealTimeUpdateSpeedMs(uint16_t value) { Set(L"RealTimeUpdateSpeedMs", value); }

        hstring StartPage() { return Get(L"StartPage", hstring{ L"ProcessesPage" }); }
        void StartPage(hstring const& value) { Set(L"StartPage", value); }

        bool MemoryDumpCaptureHypervisorPages() { return Get(L"MemoryDumpCaptureHypervisorPages", false); }
        void MemoryDumpCaptureHypervisorPages(bool value) { Set(L"MemoryDumpCaptureHypervisorPages", value); }

        bool MemoryDumpAbortIfInsufficientMemory() { return Get(L"MemoryDumpAbortIfInsufficientMemory", false); }
        void MemoryDumpAbortIfInsufficientMemory(bool value) { Set(L"MemoryDumpAbortIfInsufficientMemory", value); }

        bool MemoryDumpIncludeNonessentialPages() { return Get(L"MemoryDumpIncludeNonessentialPages", false); }
        void MemoryDumpIncludeNonessentialPages(bool value) { Set(L"MemoryDumpIncludeNonessentialPages", value); }

        bool MemoryDumpCaptureUserPages() { return Get(L"MemoryDumpCaptureUserPages", false); }
        void MemoryDumpCaptureUserPages(bool value) { Set(L"MemoryDumpCaptureUserPages", value); }

        bool AppWindowAlwaysOnTop() { return Get(L"AppWindowAlwaysOnTop", false); }
        void AppWindowAlwaysOnTop(bool value) { Set(L"AppWindowAlwaysOnTop", value); }

        bool AppWindowMinimizeOnUse() { return Get(L"AppWindowMinimizeOnUse", false); }
        void AppWindowMinimizeOnUse(bool value) { Set(L"AppWindowMinimizeOnUse", value); }

        bool AppWindowHideWhenMinimized() { return Get(L"AppWindowHideWhenMinimized", false); }
        void AppWindowHideWhenMinimized(bool value) { Set(L"AppWindowHideWhenMinimized", value); }
    };
}
FACTORY(winrt::SystemExplorer::Core::Settings, GeneralSettings);