#pragma once

#include "Views/Pages/Settings/UserPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct UserPage : UserPageT<UserPage>
    {
        UserPage()
        {
           
        }
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct UserPage : UserPageT<UserPage, implementation::UserPage>
    {
    };
}
