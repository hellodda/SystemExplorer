#pragma once

#include "Core/Data/Items/RemoteComputerItem.g.h"

#include <wil/cppwinrt_authoring.h>

namespace winrt::SystemExplorer::Core::Data::Items::implementation
{
    struct RemoteComputerItem : RemoteComputerItemT<RemoteComputerItem>
    {
        RemoteComputerItem() = default;

        wil::single_threaded_rw_property<winrt::hstring> Name;
        wil::single_threaded_rw_property<winrt::hstring> ReadableName;
        wil::single_threaded_rw_property<winrt::hstring> IpAddress;
    };
}

namespace winrt::SystemExplorer::Core::Data::Items::factory_implementation
{
    struct RemoteComputerItem : RemoteComputerItemT<RemoteComputerItem, implementation::RemoteComputerItem>
    {
    };
}
