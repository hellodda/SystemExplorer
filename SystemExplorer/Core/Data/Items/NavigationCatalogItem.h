#pragma once

#include "Core/Data/Items/NavigationCatalogItem.g.h"

#include <wil/cppwinrt_authoring.h>

namespace winrt::SystemExplorer::Core::Data::Items::implementation
{
    struct NavigationCatalogItem : NavigationCatalogItemT<NavigationCatalogItem>
    {
        NavigationCatalogItem() = default;
        NavigationCatalogItem(winrt::hstring const& name, winrt::hstring const& nameId, winrt::Windows::Foundation::Uri const& uri)
            : Name(name), NameId(nameId), Uri(uri) {}

        [[nodiscard]] winrt::hstring ToString() { return Name; };

        wil::single_threaded_rw_property<winrt::hstring> Name;
        wil::single_threaded_rw_property<winrt::hstring> NameId;
        wil::single_threaded_rw_property<winrt::Windows::Foundation::Uri> Uri = nullptr;
        wil::single_threaded_rw_property<winrt::Microsoft::UI::Xaml::Controls::IconSource> Icon = nullptr;
        wil::single_threaded_rw_property<winrt::Windows::UI::Xaml::Interop::TypeName> TypeName;
        wil::single_threaded_rw_property<winrt::Windows::Foundation::Collections::IVector<winrt::hstring>> Tags = winrt::single_threaded_vector<winrt::hstring>();
    };
}

namespace winrt::SystemExplorer::Core::Data::Items::factory_implementation
{
    struct NavigationCatalogItem : NavigationCatalogItemT<NavigationCatalogItem, implementation::NavigationCatalogItem>
    {
    };
}
