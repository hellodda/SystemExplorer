#pragma once

#include "ViewModels/AppearanceViewModel.g.h"
#include <Data/Factories/AppThemeResourceFactory.h>

namespace winrt::SystemExplorer::ViewModels::implementation
{
    using namespace winrt::Windows::Foundation::Collections;

    struct AppearanceViewModel : AppearanceViewModelT<AppearanceViewModel>
    {
        AppearanceViewModel() = default;

        wil::single_threaded_property<IObservableVector<SystemExplorer::Models::Items::AppThemeResourceItem>> AppThemeResources =
            Data::Factories::AppThemeResourceFactory::AppThemeResources();

    private:
        void selectBackgroundImage() {};
        void removeBackgroundImage() {};

    };
}

namespace winrt::SystemExplorer::ViewModels::factory_implementation
{
    struct AppearanceViewModel : AppearanceViewModelT<AppearanceViewModel, implementation::AppearanceViewModel>
    {
    };
}
