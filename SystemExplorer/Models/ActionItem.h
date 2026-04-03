#pragma once

#include "Models/ActionItem.g.h"

#include <wil/cppwinrt_authoring.h>

namespace winrt::SystemExplorer::Models::implementation
{
    struct ActionItem : ActionItemT<ActionItem>
    {
        ActionItem() = default;
        
		wil::single_threaded_rw_property<ActionStatus> Status{ SystemExplorer::Models::ActionStatus::None };
		wil::single_threaded_rw_property<hstring> Message{ L"" };
    };
}

namespace winrt::SystemExplorer::Models::factory_implementation
{
    struct ActionItem : ActionItemT<ActionItem, implementation::ActionItem>
    {
    };
}
