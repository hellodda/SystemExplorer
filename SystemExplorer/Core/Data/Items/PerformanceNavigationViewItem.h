#pragma once

#include "Core/Data/Items/PerformanceNavigationViewItem.g.h"

#include <factory.h>

namespace winrt::SystemExplorer::Core::Data::Items::implementation
{
    struct PerformanceNavigationViewItem : PerformanceNavigationViewItemT<PerformanceNavigationViewItem>, wil::notify_property_changed_base<PerformanceNavigationViewItem>
    {
        PerformanceNavigationViewItem() = default;

        wil::single_threaded_rw_property<hstring> Header;

        WIL_NOTIFYING_PROPERTY(hstring, Description, L"");
        WIL_NOTIFYING_PROPERTY(hstring, SubDescription, L"");
    };
}
FACTORY(winrt::SystemExplorer::Core::Data::Items, PerformanceNavigationViewItem);