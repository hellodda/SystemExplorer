#pragma once

#include "Views/Pages/Settings/RemoteAccessPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct RemoteAccessPage : RemoteAccessPageT<RemoteAccessPage>
    {
        RemoteAccessPage() = default;
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct RemoteAccessPage : RemoteAccessPageT<RemoteAccessPage, implementation::RemoteAccessPage>
    {
    };
}
