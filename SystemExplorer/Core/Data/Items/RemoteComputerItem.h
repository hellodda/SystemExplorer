#pragma once

#include "Core/Data/Items/RemoteComputerItem.g.h"

namespace winrt::SystemExplorer::Core::Data::Items::implementation
{
    struct RemoteComputerItem : RemoteComputerItemT<RemoteComputerItem>
    {
        RemoteComputerItem() = default;
    };
}

namespace winrt::SystemExplorer::Core::Data::Items::factory_implementation
{
    struct RemoteComputerItem : RemoteComputerItemT<RemoteComputerItem, implementation::RemoteComputerItem>
    {
    };
}
