#pragma once

#include "Core/Data/Items/AppLanguageItem.g.h"

#ifdef __INTELLISENSE__
#include <wil/cppwinrt_authoring.h>
#endif
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
