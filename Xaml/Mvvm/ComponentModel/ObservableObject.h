#pragma once

#include "Mvvm/ComponentModel/ObservableObject.g.h"
#include <wil/cppwinrt_authoring.h>

namespace winrt::SystemExplorer::Xaml::Mvvm::ComponentModel::implementation
{
    struct ObservableObject : ObservableObjectT<ObservableObject>, wil::notify_property_changed_base<ObservableObject>
    {
        ObservableObject() = default;
    };
}

namespace winrt::SystemExplorer::Xaml::Mvvm::ComponentModel::factory_implementation
{
    struct ObservableObject : ObservableObjectT<ObservableObject, implementation::ObservableObject>
    {
    };
}
