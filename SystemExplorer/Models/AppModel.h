#pragma once

#include "Models/AppModel.g.h"

#include <factory.h>
#include <property.h>

namespace winrt::SystemExplorer::Models::implementation
{
    struct AppModel : AppModelT<AppModel>, wil::notify_property_changed_base<AppModel>
    {
        AppModel();
        
        DECLARE_ONLY_GETTER(float, AppWindowDPI, 0.0f);
    };
}
FACTORY(winrt::SystemExplorer::Models, AppModel);
