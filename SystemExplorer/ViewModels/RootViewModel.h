#pragma once

#include "ViewModels/RootViewModel.g.h"

namespace winrt::SystemExplorer::ViewModels::implementation
{
    struct RootViewModel : RootViewModelT<RootViewModel>
    {
        RootViewModel() = default;
    };
}

namespace winrt::SystemExplorer::ViewModels::factory_implementation
{
    struct RootViewModel : RootViewModelT<RootViewModel, implementation::RootViewModel>
    {
    };
}
