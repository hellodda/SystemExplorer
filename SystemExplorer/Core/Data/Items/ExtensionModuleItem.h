#pragma once

#include "Core/Data/Items/ExtensionModuleItem.g.h"

#ifdef __INTELLISENSE__
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::SystemExplorer::Core::Data::Items::implementation
{
    struct ExtensionModuleItem : ExtensionModuleItemT<ExtensionModuleItem>
    {
        ExtensionModuleItem() = default;

        wil::single_threaded_rw_property<winrt::hstring> Name;
        wil::single_threaded_rw_property<winrt::hstring> Author;
        wil::single_threaded_rw_property<winrt::hstring> Description;

        wil::single_threaded_rw_property<winrt::guid> ModuleGuid;
    };
}

namespace winrt::SystemExplorer::Core::Data::Items::factory_implementation
{
    struct ExtensionModuleItem : ExtensionModuleItemT<ExtensionModuleItem, implementation::ExtensionModuleItem>
    {
    };
}
