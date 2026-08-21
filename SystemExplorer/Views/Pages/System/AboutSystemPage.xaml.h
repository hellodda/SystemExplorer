#pragma once

#include "Views/Pages/System/AboutSystemPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::System::implementation
{
    struct AboutSystemPage : AboutSystemPageT<AboutSystemPage>
    {
        AboutSystemPage()
        {
           
        }
    };
}

namespace winrt::SystemExplorer::Views::Pages::System::factory_implementation
{
    struct AboutSystemPage : AboutSystemPageT<AboutSystemPage, implementation::AboutSystemPage>
    {
    };
}
