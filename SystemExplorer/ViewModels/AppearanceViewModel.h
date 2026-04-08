#pragma once

#include "ViewModels/AppearanceViewModel.g.h"
#include <Core/Data/Factories/AppThemeResourceFactory.h>
#include <Core/Settings/UserSettings.h>
#include <Core/Services/AppResourcesService.h>

namespace winrt::SystemExplorer::ViewModels::implementation
{
    using namespace winrt::Windows::Foundation::Collections;

    struct AppearanceViewModel : AppearanceViewModelT<AppearanceViewModel>, wil::notify_property_changed_base<AppearanceViewModel>
    {
        AppearanceViewModel() = default;

        wil::single_threaded_property<IObservableVector<SystemExplorer::Models::Items::AppThemeResourceItem>> AppThemeResources =
            Core::Data::Factories::AppThemeResourceFactory::AppThemeResources();

        hstring AppThemeBackgroundColor() const noexcept
        {
           return Core::Settings::UserSettings::Instance().AppearanceSettings().AppThemeBackgroundColor();
        }
        void AppThemeBackgroundColor(hstring const& value)
        {
            auto color = XamlToolkit::WinUI::Helpers::ColorHelper::ToColor(value);

            Core::Services::AppResourcesService::Instance().SetAppThemeBackgroundColor(color);
            Core::Settings::UserSettings::Instance().AppearanceSettings().AppThemeBackgroundColor(value);

            RaisePropertyChanged(L"AppThemeBackgroundColor");
        }
        SystemExplorer::Models::Items::AppThemeResourceItem SelectedAppThemeResources() const noexcept
        {
            return selectedAppThemeResources_;
        }
        void SelectedAppThemeResources(SystemExplorer::Models::Items::AppThemeResourceItem const& value)
        {
            if (value)
            {
                AppThemeBackgroundColor(value.BackgroundColor());
                selectedAppThemeResources_ = value;
                RaisePropertyChanged(L"SelectedAppThemeResources");
            }
        }

    private:
        SystemExplorer::Models::Items::AppThemeResourceItem selectedAppThemeResources_{ L"Default", L"#00000000" };
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
