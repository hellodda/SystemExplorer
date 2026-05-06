#pragma once

#include "Core/Data/Items/AppThemeResourceItem.g.h"
#include <factory.h>

namespace winrt::SystemExplorer::Core::Data::Items::implementation
{
    struct AppThemeResourceItem : AppThemeResourceItemT<AppThemeResourceItem>
    {
        AppThemeResourceItem() = default;
        AppThemeResourceItem(hstring const& name, hstring const& backgroundColor)
            : Name(name), BackgroundColor(backgroundColor) {}

        wil::single_threaded_rw_property<hstring> Name;
        wil::single_threaded_rw_property<hstring> BackgroundColor;
    };
}
FACTORY(winrt::SystemExplorer::Core::Data::Items, AppThemeResourceItem);