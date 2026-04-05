#pragma once

#include "Models/Items/AppThemeResourceItem.g.h"

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

namespace winrt::SystemExplorer::Models::Items::factory_implementation
{
    struct AppThemeResourceItem : AppThemeResourceItemT<AppThemeResourceItem, implementation::AppThemeResourceItem>
    {
    };
}
