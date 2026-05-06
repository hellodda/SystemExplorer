#pragma once

#include "Core/Data/Items/AppLanguageItem.g.h"
#include <factory.h>

namespace winrt::SystemExplorer::Core::Data::Items::implementation
{
    struct AppLanguageItem : AppLanguageItemT<AppLanguageItem>
    {
        AppLanguageItem(hstring const& code, bool systemDefault);

        wil::single_threaded_rw_property<hstring> Code;
		wil::single_threaded_rw_property<hstring> Name;
    };
}
FACTORY(winrt::SystemExplorer::Core::Data::Items, AppLanguageItem);
