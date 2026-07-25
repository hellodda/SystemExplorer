#pragma once

#include "ViewModels/ViewModelBase.g.h"

#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif
#include <factory.h>

namespace winrt::SystemExplorer::ViewModels::implementation
{
    struct ViewModelBase : ViewModelBaseT<ViewModelBase>, wil::notify_property_changed_base<ViewModelBase>
    {
        ViewModelBase() = default;
    };
}
FACTORY(winrt::SystemExplorer::ViewModels, ViewModelBase);