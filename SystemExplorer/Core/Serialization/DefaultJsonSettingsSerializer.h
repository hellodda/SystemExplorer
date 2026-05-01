#pragma once

#include "Core/Serialization/DefaultJsonSettingsSerializer.g.h"
#include <factory.h>

namespace winrt::SystemExplorer::Core::Serialization::implementation
{
    struct DefaultJsonSettingsSerializer : DefaultJsonSettingsSerializerT<DefaultJsonSettingsSerializer>
    {
        DefaultJsonSettingsSerializer() = default;

        hstring ExportSettingsToJson() const noexcept;
        void ImportSettingsFromJson(hstring const& json) noexcept;
    };
}
FACTORY(winrt::SystemExplorer::Core::Serialization, DefaultJsonSettingsSerializer);