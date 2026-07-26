#pragma once

#include "Core/Data/Models/AppModel.g.h"


namespace winrt::SystemExplorer::Core::Data::Models::implementation
{
    struct AppModel : AppModelT<AppModel>, wil::notify_property_changed_base<AppModel>
    {
        AppModel() = default;
        
        DECLARE_ONLY_GETTER(float, AppWindowDPI, 0.0f);
    };
}
FACTORY(winrt::SystemExplorer::Core::Data::Models, AppModel);
