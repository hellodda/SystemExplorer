#pragma once

#include "Views/Flyouts/ActionsCenterFlyout.g.h"

namespace winrt::SystemExplorer::Views::Flyouts::implementation
{
    struct ActionsCenterFlyout : ActionsCenterFlyoutT<ActionsCenterFlyout>
    {
        ActionsCenterFlyout()
        {
    
        }
    };
}

namespace winrt::SystemExplorer::Views::Flyouts::factory_implementation
{
    struct ActionsCenterFlyout : ActionsCenterFlyoutT<ActionsCenterFlyout, implementation::ActionsCenterFlyout>
    {
    };
}
