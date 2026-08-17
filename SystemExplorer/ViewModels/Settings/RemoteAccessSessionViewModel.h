#pragma once

#include "ViewModels/Settings/RemoteAccessSessionViewModel.g.h"

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    struct RemoteAccessSessionViewModel : RemoteAccessSessionViewModelT<RemoteAccessSessionViewModel>
    {
        RemoteAccessSessionViewModel() = default;
    };
}

namespace winrt::SystemExplorer::ViewModels::Settings::factory_implementation
{
    struct RemoteAccessSessionViewModel : RemoteAccessSessionViewModelT<RemoteAccessSessionViewModel, implementation::RemoteAccessSessionViewModel>
    {
    };
}
