#pragma once

#include "Core/Settings/UserSettings.g.h"
#include "../Serialization/DefaultJsonSettingsSerializer.h"
#include "AdvancedSettings.h"
#include "AppearanceSettings.h"

namespace winrt::SystemExplorer::Core::Settings::implementation
{
    struct UserSettings : UserSettingsT<UserSettings, Core::Serialization::implementation::DefaultJsonSettingsSerializer>
    {
        UserSettings() = default;
        static Settings::UserSettings Instance();

        wil::single_threaded_property<Settings::AdvancedSettings> AdvancedSettings;
        wil::single_threaded_property<Settings::AppearanceSettings> AppearanceSettings;
    };
}

namespace winrt::SystemExplorer::Core::Settings::factory_implementation
{
    struct UserSettings : UserSettingsT<UserSettings, implementation::UserSettings>
    {
    };
}
