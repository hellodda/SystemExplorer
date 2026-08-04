#pragma once

#include "Core/Data/Items/NavigationBreadcrumbBarItem.g.h"
#include <wil/cppwinrt_authoring.h>

namespace winrt
{
    using namespace winrt::Windows::UI::Xaml::Interop;
}

namespace winrt::SystemExplorer::Core::Data::Items::implementation
{
    struct NavigationBreadcrumbBarItem : NavigationBreadcrumbBarItemT<NavigationBreadcrumbBarItem>
    {
        NavigationBreadcrumbBarItem() = default;
        NavigationBreadcrumbBarItem(winrt::hstring const& header, winrt::TypeName const& pageType)
            : Header(header), PageType(pageType) {}

        wil::single_threaded_rw_property<winrt::hstring> Header;
        wil::single_threaded_rw_property<winrt::TypeName> PageType;
    };
}

namespace winrt::SystemExplorer::Core::Data::Items::factory_implementation
{
    struct NavigationBreadcrumbBarItem : NavigationBreadcrumbBarItemT<NavigationBreadcrumbBarItem, implementation::NavigationBreadcrumbBarItem>
    {
    };
}
