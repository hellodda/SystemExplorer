#pragma once

#include "ViewModels/Settings/AppearanceViewModel.g.h"
#include <Core/Data/Factories/AppThemeResourceFactory.h>
#include <Core/Settings/UserSettings.h>
#include <macro.h>

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Windows::Foundation;

    using namespace winrt::SystemExplorer::Models::Items;
    using namespace winrt::SystemExplorer::Core;

    struct AppearanceViewModel : AppearanceViewModelT<AppearanceViewModel>, wil::notify_property_changed_base<AppearanceViewModel>
    {
        AppearanceViewModel();

        wil::single_threaded_property<IObservableVector<AppThemeResourceItem>> AppThemeResources =
            Data::Factories::AppThemeResourceFactory::AppThemeResources();

        wil::single_threaded_property<IVector<IInspectable>> BackdropMaterialTypes =
            single_threaded_vector<IInspectable>();

        wil::single_threaded_property<IVector<IInspectable>> AppThemes =
            single_threaded_vector<IInspectable>();

        wil::single_threaded_rw_property<int32_t> SelectedBackdropMaterialIndex;

        DECLARE_PROPERTY(hstring, AppThemeBackgroundColor);
        DECLARE_PROPERTY(AppThemeResourceItem, SelectedAppThemeResources);
        DECLARE_PROPERTY(IInspectable, SelectedBackdropMaterial);
        DECLARE_PROPERTY(int32_t, SelectedAppThemeIndex);
    private:
        Core::Settings::IAppearanceSettings settings_ = Core::Settings::UserSettings::Instance().AppearanceSettings();

        hstring BackdropMaterialType(Data::Enums::BackdropMaterialType value);
        Data::Enums::BackdropMaterialType BackdropMaterialType(hstring const& value);

        void updateSelectedResource();
        void updateSelectedBackdropMaterial();
        void updateSelectedAppTheme();
    };
}

FACTORY(winrt::SystemExplorer::ViewModels::Settings, AppearanceViewModel);
