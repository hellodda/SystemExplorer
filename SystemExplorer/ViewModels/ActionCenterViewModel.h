#pragma once

#include "ViewModels/ActionCenterViewModel.g.h"
#include <wil/cppwinrt_authoring.h>
#include "ViewModelBase.h"

namespace winrt::SystemExplorer::ViewModels::implementation
{
    using namespace winrt::Windows::Foundation::Collections;

    struct ActionCenterViewModel : ActionCenterViewModelT<ActionCenterViewModel>, BindableViewModelBase<ActionCenterViewModel>
    {
        ActionCenterViewModel() = default;

		wil::single_threaded_property<IObservableVector<Models::ActionItem>> Actions{ };
    };
}

namespace winrt::SystemExplorer::ViewModels::factory_implementation
{
    struct ActionCenterViewModel : ActionCenterViewModelT<ActionCenterViewModel, implementation::ActionCenterViewModel>
    {
    };
}
