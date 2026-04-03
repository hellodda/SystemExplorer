#pragma once

#include "ViewModels/Pages/RootViewModel.g.h"

namespace winrt::SystemExplorer::ViewModels::Pages::implementation
{
    struct RootViewModel : RootViewModelT<RootViewModel>
    {
        RootViewModel() = default;
    };
}

namespace winrt::SystemExplorer::ViewModels::Pages::factory_implementation
{
    struct RootViewModel : RootViewModelT<RootViewModel, implementation::RootViewModel>
    {
    };
}
