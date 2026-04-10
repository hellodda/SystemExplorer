#pragma once

#include "Core/Serialization/SettingsBase.g.h"
#include <winrt/Windows.Storage.h>
#include <winrt/Microsoft.Windows.Storage.h>
#include <Core/Data/EventArguments/SettingChangedEventArgs.h>
#include <unordered_set>
#include <shared_mutex>

namespace winrt::SystemExplorer::Core::Serialization::implementation
{
    using namespace winrt::Windows::Foundation::Collections;

    struct SettingsBase : SettingsBaseT<SettingsBase>, wil::notify_property_changed_base<SettingsBase>
    {
        SettingsBase() : Values(Microsoft::Windows::Storage::ApplicationData::GetDefault().LocalSettings().Values()) {}
        ~SettingsBase() { Save(); } // lyuti stub

        wil::single_threaded_property<IPropertySet> Values;
        wil::typed_event<IInspectable, Core::Data::EventArguments::SettingChangedEventArgs> SettingChanged;

        void Save();
    protected:
        template<typename T>
        [[nodiscard]] T Get(hstring const& key, T defaultValue)
        {
            auto value = GetCached(key);
            return value ? unbox_value<T>(value) : defaultValue;
        }
        [[nodiscard]] IInspectable Get(hstring const& key);

        template<typename T>
        void Set(hstring const& key, T const& value)
        {
            auto boxed = BoxValue(value);
            cache_[key] = boxed;
            dirtyKeys_.insert(key);

            SettingChanged.invoke(*this, Core::Data::EventArguments::SettingChangedEventArgs{ key, boxed });
        }
    private:
        [[nodiscard]] IInspectable GetCached(hstring const& key);

        std::unordered_map<hstring, IInspectable> cache_;
        std::unordered_set<hstring> dirtyKeys_;

        template<typename T>
        [[nodiscard]] IInspectable BoxValue(T value)
        {
            if constexpr (std::is_enum_v<T>) return box_value(static_cast<std::underlying_type_t<T>>(value));
            return box_value(value);
        }
    };
}

namespace winrt::SystemExplorer::Core::Serialization::factory_implementation
{
    struct SettingsBase : SettingsBaseT<SettingsBase, implementation::SettingsBase>
    {
    };
}
