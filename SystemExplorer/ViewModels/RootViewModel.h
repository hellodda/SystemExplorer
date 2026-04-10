#pragma once

#include "ViewModels/RootViewModel.g.h"

namespace winrt::SystemExplorer::ViewModels::implementation
{
    struct RootViewModel : RootViewModelT<RootViewModel>
    {
        RootViewModel() = default;
    };
}

FACTORY(winrt::SystemExplorer::ViewModels, RootViewModel);