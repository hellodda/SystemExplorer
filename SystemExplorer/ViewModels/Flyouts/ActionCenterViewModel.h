#pragma once

#include "ViewModels/Flyouts/ActionCenterViewModel.g.h"
#include <Models/ActionItem.h>
#include <wil/cppwinrt_authoring.h>

#include <ViewModels/ViewModelBase.h>

namespace winrt::SystemExplorer::ViewModels::Flyouts::implementation
{
    using namespace winrt::Windows::Foundation::Collections;

    struct ActionCenterViewModel : ActionCenterViewModelT<ActionCenterViewModel>, BindableViewModelBase<ActionCenterViewModel>
    {
        ActionCenterViewModel() = default;

		wil::single_threaded_property<IObservableVector<Models::ActionItem>> Actions{ winrt::single_threaded_observable_vector<Models::ActionItem>() };
    };
}

namespace winrt::SystemExplorer::ViewModels::Flyouts::factory_implementation
{
    struct ActionCenterViewModel : ActionCenterViewModelT<ActionCenterViewModel, implementation::ActionCenterViewModel>
    {
    };
}
