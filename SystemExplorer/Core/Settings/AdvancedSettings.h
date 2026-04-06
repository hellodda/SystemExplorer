#pragma once

#include "Core/Settings/AdvancedSettings.g.h"
#include <Core/Serialization/SettingsBase.h>

namespace winrt::SystemExplorer::Core::Settings::implementation
{
    struct AdvancedSettings : AdvancedSettingsT<AdvancedSettings, Serialization::implementation::SettingsBase>
    {
        AdvancedSettings() = default;

        bool AdvancedAiAgent() { return Get<bool>(L"AdvancedAiAgent", false); }
        void AdvancedAiAgent(bool value) { Set(L"AdvancedAiAgent", value); }
    };
}

namespace winrt::SystemExplorer::Core::Settings::factory_implementation
{
    struct AdvancedSettings : AdvancedSettingsT<AdvancedSettings, implementation::AdvancedSettings>
    {
        static auto CreateInstance()
        {
            static auto instance = winrt::make<implementation::AdvancedSettings>();
            return instance;
        }
    };
}
