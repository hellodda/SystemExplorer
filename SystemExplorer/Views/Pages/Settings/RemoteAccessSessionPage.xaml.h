#pragma once

#include "Views/Pages/Settings/RemoteAccessSessionPage.g.h"

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct RemoteAccessSessionPage : RemoteAccessSessionPageT<RemoteAccessSessionPage>
    {
        RemoteAccessSessionPage() = default;
    };
}

namespace winrt::SystemExplorer::Views::Pages::Settings::factory_implementation
{
    struct RemoteAccessSessionPage : RemoteAccessSessionPageT<RemoteAccessSessionPage, implementation::RemoteAccessSessionPage>
    {
    };
}
