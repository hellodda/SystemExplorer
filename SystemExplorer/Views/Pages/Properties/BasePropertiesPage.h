#pragma once
#include "Views/Pages/Properties/BasePropertiesPage.g.h"

#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Properties::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Navigation;

    struct BasePropertiesPage : BasePropertiesPageT<BasePropertiesPage>
    {
        BasePropertiesPage() = default;

        void OnNavigatedTo(NavigationEventArgs const& args);
        void OnNavigatedFrom(NavigationEventArgs const& args);
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Properties, BasePropertiesPage);
