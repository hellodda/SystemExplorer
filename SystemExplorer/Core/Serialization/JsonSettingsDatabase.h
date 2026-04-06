#pragma once

#include "Core/Serialization/JsonSettingsDatabase.g.h"

namespace winrt::SystemExplorer::Core::Serialization::implementation
{
    using namespace winrt::Windows::Foundation;

    struct JsonSettingsDatabase : JsonSettingsDatabaseT<JsonSettingsDatabase>
    {
        JsonSettingsDatabase() = default;

        IInspectable GetValue(hstring const& key, IInspectable const& defaultValue);

        bool SetValue(hstring const& key, IInspectable const& newValue);

        bool RemoveKey(hstring const& key);

        bool ImportSettings(IInspectable const& settings);

        IInspectable ExportSettings();

    private:

    };
}

namespace winrt::SystemExplorer::Core::Serialization::factory_implementation
{
    struct JsonSettingsDatabase : JsonSettingsDatabaseT<JsonSettingsDatabase, implementation::JsonSettingsDatabase>
    {
    };
}
