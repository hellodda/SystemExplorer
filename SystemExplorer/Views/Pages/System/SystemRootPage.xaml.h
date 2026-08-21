#pragma once

#include "Views/Pages/System/SystemRootPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::System::implementation
{
    struct SystemRootPage : SystemRootPageT<SystemRootPage>
    {
        SystemRootPage()
        {
        
        }
    };
}

namespace winrt::SystemExplorer::Views::Pages::System::factory_implementation
{
    struct SystemRootPage : SystemRootPageT<SystemRootPage, implementation::SystemRootPage>
    {
    };
}
