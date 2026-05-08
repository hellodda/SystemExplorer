#pragma once

#include "Core/Data/Items/BaseProperties.g.h"
#include "../Models/SelectedItemsPropertiesViewModel.h"

#include <factory.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::SystemExplorer::Core::Data::Items::implementation
{
    using namespace winrt::SystemExplorer::Core::Data::Models;

    struct BaseProperties : BasePropertiesT<BaseProperties>
    {
        BaseProperties() = default;

        wil::single_threaded_rw_property<SelectedItemsPropertiesViewModel> ViewModel;

        void GetBaseProperties() {};
    };
}
FACTORY(winrt::SystemExplorer::Core::Data::Items, BaseProperties);