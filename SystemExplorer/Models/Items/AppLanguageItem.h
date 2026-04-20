#pragma once

#include "Models/Items/AppLanguageItem.g.h"

namespace winrt::SystemExplorer::Models::Items::implementation
{
    struct AppLanguageItem : AppLanguageItemT<AppLanguageItem>
    {
        AppLanguageItem(hstring const& code, bool systemDefault);

        wil::single_threaded_rw_property<hstring> Code;
		wil::single_threaded_rw_property<hstring> Name;
    };
}
FACTORY(winrt::SystemExplorer::Models::Items, AppLanguageItem);
