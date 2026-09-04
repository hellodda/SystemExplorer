#pragma once

#include "Core/Data/Items/ObjectPairItem.g.h"

#ifdef __INTELLISENSE__
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace winrt::Windows::Foundation;
}

namespace winrt::SystemExplorer::Core::Data::Items::implementation
{
    struct ObjectPairItem : ObjectPairItemT<ObjectPairItem>
    {
        ObjectPairItem() = default;
        ObjectPairItem(winrt::IInspectable const& first, winrt::IInspectable const& second)
            : First(first), Second(second) {}

        wil::single_threaded_rw_property<winrt::IInspectable> First;
        wil::single_threaded_rw_property<winrt::IInspectable> Second;
    };
}

namespace winrt::SystemExplorer::Core::Data::Items::factory_implementation
{
    struct ObjectPairItem : ObjectPairItemT<ObjectPairItem, implementation::ObjectPairItem>
    {
    };
}
