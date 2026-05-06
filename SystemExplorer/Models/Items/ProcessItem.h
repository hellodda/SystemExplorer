#pragma once

#include "Models/Items/ProcessItem.g.h"

#include <factory.h>

namespace winrt::SystemExplorer::Models::Items::implementation
{
    struct ProcessItem : ProcessItemT<ProcessItem>
    {
        ProcessItem() = default;

        wil::single_threaded_rw_property<uint32_t> Pid;
    };
}
FACTORY(winrt::SystemExplorer::Models::Items, ProcessItem);

