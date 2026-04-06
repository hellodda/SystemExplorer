#pragma once

#include "Core/Settings/AppearanceSettings.g.h"
#include <Core/Serialization/SettingsBase.h>

namespace winrt::SystemExplorer::Core::Settings::implementation
{
    struct AppearanceSettings : AppearanceSettingsT<AppearanceSettings, Serialization::implementation::SettingsBase>
    {
        AppearanceSettings() = default;

        int32_t TestProp() { return Get(L"TestProp", 1); }
        void TestProp(int32_t value) { Set(L"TestProp", value); }
    };
}

namespace winrt::SystemExplorer::Core::Settings::factory_implementation
{
    struct AppearanceSettings : AppearanceSettingsT<AppearanceSettings, implementation::AppearanceSettings>
    {
    };
}
