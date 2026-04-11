#pragma once

#include "Core/Data/EventArguments/SettingChangedEventArgs.g.h"
#include <macro.h>

namespace winrt::SystemExplorer::Core::Data::EventArguments::implementation
{
    using namespace winrt::Windows::Foundation;

    struct SettingChangedEventArgs : SettingChangedEventArgsT<SettingChangedEventArgs>
    {
        SettingChangedEventArgs() = default;
        SettingChangedEventArgs(hstring const& settingName, IInspectable const& newValue)
            : SettingName(settingName), NewValue(newValue) {}

        wil::single_threaded_rw_property<hstring> SettingName;
        wil::single_threaded_rw_property<IInspectable> NewValue;
    };
}
FACTORY(winrt::SystemExplorer::Core::Data::EventArguments, SettingChangedEventArgs);
