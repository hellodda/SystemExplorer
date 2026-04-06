#pragma once

#include "Core/Serialization/SettingsBase.g.h"
#include <winrt/Windows.Storage.h>
#include <winrt/Microsoft.Windows.Storage.h>

namespace winrt::SystemExplorer::Core::Serialization::implementation
{
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Microsoft::UI::Xaml::Data;

    struct SettingsBase : SettingsBaseT<SettingsBase>, wil::notify_property_changed_base<SettingsBase>
    {
        SettingsBase()
        {
            Initialize(Windows::Storage::ApplicationData::Current().LocalSettings().Values());
        }

        wil::single_threaded_property<IPropertySet> Values;

    protected:

        template<typename T>
        T Get(hstring const& key, T defaultValue)
        {
            auto props = Values(); 
            if (!props)            
                return defaultValue;

            auto it = props.TryLookup(key);
            return it ? unbox_value<T>(it) : defaultValue;
        }

        template<typename T>
        void Set(hstring const& key, T const& value)
        {
            auto props = Values(); 
            if (props)            
            {
                props.Insert(key, box_value(value));
            }
        }
    private:
        void Initialize(IPropertySet const& values);
    };
}

namespace winrt::SystemExplorer::Core::Serialization::factory_implementation
{
    struct SettingsBase : SettingsBaseT<SettingsBase, implementation::SettingsBase>
    {
    };
}
