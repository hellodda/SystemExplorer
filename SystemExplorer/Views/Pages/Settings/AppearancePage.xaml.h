#pragma once
#include "Views/Pages/Settings/AppearancePage.g.h"
#include <ViewModels/Settings/AppearanceViewModel.h>
#include <Core/Settings/UserSettings.h>
#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml::Controls;

    struct AppearancePage : AppearancePageT<AppearancePage>
    {
        AppearancePage()
        {
            
        }
        wil::single_threaded_property<ViewModels::Settings::AppearanceViewModel> ViewModel;
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Settings, AppearancePage);