#include "pch.h"
#include "DefaultJsonSettingsSerializer.h"
#if __has_include("Core/Serialization/DefaultJsonSettingsSerializer.g.cpp")
#include "Core/Serialization/DefaultJsonSettingsSerializer.g.cpp"
#endif
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Data.Json.h>
#include <winrt/Microsoft.Windows.Storage.h> 

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::Data::Json;

namespace AppStorage = winrt::Microsoft::Windows::Storage;

namespace winrt::SystemExplorer::Core::Serialization::implementation
{
    hstring DefaultJsonSettingsSerializer::ExportSettingsToJson() const noexcept
    {
        auto settings = AppStorage::ApplicationData::GetDefault().LocalSettings().Values();
        JsonObject jsonObject;

        for (auto const& pair : settings)
        {
            hstring key = pair.Key();
            IInspectable value = pair.Value();

            if (!value) continue;

            auto propValue = value.as<IPropertyValue>();
            if (!propValue) continue;

            PropertyType type = propValue.Type();

            switch (type)
            {
            case PropertyType::String:
                jsonObject.SetNamedValue(key, JsonValue::CreateStringValue(unbox_value<hstring>(value)));
                break;
            case PropertyType::Int32:
                jsonObject.SetNamedValue(key, JsonValue::CreateNumberValue(unbox_value<int32_t>(value)));
                break;
            case PropertyType::UInt32:
                jsonObject.SetNamedValue(key, JsonValue::CreateNumberValue(unbox_value<uint32_t>(value)));
                break;
            case PropertyType::Int64:
                jsonObject.SetNamedValue(key, JsonValue::CreateNumberValue(unbox_value<int64_t>(value)));
                break;
            case PropertyType::Double:
                jsonObject.SetNamedValue(key, JsonValue::CreateNumberValue(unbox_value<double>(value)));
            case PropertyType::Single:
                jsonObject.SetNamedValue(key, JsonValue::CreateNumberValue(unbox_value<float>(value)));
                break;
            case PropertyType::Boolean:
                jsonObject.SetNamedValue(key, JsonValue::CreateBooleanValue(unbox_value<bool>(value)));
                break;
            default:
                break;
            }
        }
        return jsonObject.Stringify();
    }
    void DefaultJsonSettingsSerializer::ImportSettingsFromJson(hstring const& json) noexcept
    {
        JsonObject jsonObject{ nullptr };

        if (!JsonObject::TryParse(json, jsonObject))
            return;

        auto settings = AppStorage::ApplicationData::GetDefault().LocalSettings().Values();

        for (auto const& pair : jsonObject)
        {
            hstring key = pair.Key();
            IJsonValue jsonValue = pair.Value();

            switch (jsonValue.ValueType())
            {
            case JsonValueType::String:
                settings.Insert(key, box_value(jsonValue.GetString()));
                break;
            case JsonValueType::Number:
                settings.Insert(key, box_value(jsonValue.GetNumber()));
                break;
            case JsonValueType::Boolean:
                settings.Insert(key, box_value(jsonValue.GetBoolean()));
                break;
            case JsonValueType::Null:

                if (settings.HasKey(key))
                {
                    settings.Remove(key);
                }
                break;
            default:
                break;
            }
        }
    }
}
