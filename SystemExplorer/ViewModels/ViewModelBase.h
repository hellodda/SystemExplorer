#pragma once

#include "ViewModels/ViewModelBase.g.h"
#include <macro.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::SystemExplorer::ViewModels::implementation
{
    struct ViewModelBase : ViewModelBaseT<ViewModelBase>, wil::notify_property_changed_base<ViewModelBase>
    {
        ViewModelBase() = default;
    };
}
FACTORY(winrt::SystemExplorer::ViewModels, ViewModelBase);