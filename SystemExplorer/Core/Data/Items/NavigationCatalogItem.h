#pragma once

#include "Core/Data/Items/NavigationCatalogItem.g.h"

#include <wil/cppwinrt_authoring.h>

namespace winrt::SystemExplorer::Core::Data::Items::implementation
{
    struct NavigationCatalogItem : NavigationCatalogItemT<NavigationCatalogItem>
    {
        NavigationCatalogItem() = default;

        wil::single_threaded_rw_property<winrt::hstring> Name;
        wil::single_threaded_rw_property<winrt::hstring> NameId;
        wil::single_threaded_rw_property<winrt::Windows::Foundation::Uri> Uri;
        wil::single_threaded_rw_property<winrt::Windows::Foundation::Collections::IVector<winrt::hstring>> Tags = winrt::single_threaded_vector<winrt::hstring>();
    };
}

namespace winrt::SystemExplorer::Core::Data::Items::factory_implementation
{
    struct NavigationCatalogItem : NavigationCatalogItemT<NavigationCatalogItem, implementation::NavigationCatalogItem>
    {
    };
}
