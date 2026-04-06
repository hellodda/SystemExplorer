#pragma once
#undef CreateFile
#include "Core/Serialization/SettingsSerializer.g.h"

namespace winrt::SystemExplorer::Core::Serialization::implementation
{
    struct SettingsSerializer : SettingsSerializerT<SettingsSerializer>
    {
        SettingsSerializer() = default;

        bool CreateFile(hstring const& path);

        hstring ReadFromFile();

        bool WriteToFile(hstring const& text);
    };
}

namespace winrt::SystemExplorer::Core::Serialization::factory_implementation
{
    struct SettingsSerializer : SettingsSerializerT<SettingsSerializer, implementation::SettingsSerializer>
    {
    };
}
