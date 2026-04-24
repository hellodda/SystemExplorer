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
    };
}
FACTORY(winrt::SystemExplorer::Core::Settings, GeneralSettings);