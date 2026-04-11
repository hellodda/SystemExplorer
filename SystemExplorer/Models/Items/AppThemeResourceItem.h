#pragma once

#include "Models/Items/AppThemeResourceItem.g.h"
#include <macro.h>

namespace winrt::SystemExplorer::Models::Items::implementation
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
FACTORY(winrt::SystemExplorer::Models::Items, AppThemeResourceItem);