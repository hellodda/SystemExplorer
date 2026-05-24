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

		bool ProcessScaningModel() { return Get<bool>(L"ProcessScaningModel", false); }
		void ProcessScaningModel(bool value) { Set(L"ProcessScaningModel", value); }

        bool InStartup() { return Get<bool>(L"InStartup", false); }
        void InStartup(bool value) { Set(L"InStartup", value); }
    };
}
FACTORY(winrt::SystemExplorer::Core::Settings, AdvancedSettings);