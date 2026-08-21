#pragma once

#include "ViewModels/Settings/ExtensionModulesViewModel.g.h"

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    struct ExtensionModulesViewModel : ExtensionModulesViewModelT<ExtensionModulesViewModel>
    {
        ExtensionModulesViewModel() = default;
    };
}

namespace winrt::SystemExplorer::ViewModels::Settings::factory_implementation
{
    struct ExtensionModulesViewModel : ExtensionModulesViewModelT<ExtensionModulesViewModel, implementation::ExtensionModulesViewModel>
    {
    };
}
