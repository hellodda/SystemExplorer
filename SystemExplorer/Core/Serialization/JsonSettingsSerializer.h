#pragma once

#include "Core/Serialization/JsonSettingsSerializer.g.h"

namespace winrt::SystemExplorer::Core::Serialization::implementation
{
    using namespace winrt::Windows::Foundation;

    struct JsonSettingsSerializer : JsonSettingsSerializerT<JsonSettingsSerializer>
    {
        JsonSettingsSerializer() = default;

        hstring SerializeToJson(IInspectable const& obj) const noexcept;
        IInspectable DeserializeFromJson(hstring const& json) const noexcept;
    };
}

namespace winrt::SystemExplorer::Core::Serialization::factory_implementation
{
    struct JsonSettingsSerializer : JsonSettingsSerializerT<JsonSettingsSerializer, implementation::JsonSettingsSerializer>
    {
    };
}
