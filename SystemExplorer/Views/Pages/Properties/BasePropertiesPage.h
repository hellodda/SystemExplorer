#pragma once
#include "Views/Pages/Properties/BasePropertiesPage.g.h"

#include <Core/Data/Models/SelectedItemsPropertiesViewModel.h>
#include <Core/Data/Items/BaseProperties.h>
#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Properties::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Navigation;

    using namespace winrt::SystemExplorer::Core::Data::Models;
    using namespace winrt::SystemExplorer::Core::Data::Items;

    struct BasePropertiesPage : BasePropertiesPageT<BasePropertiesPage>
    {
        BasePropertiesPage() = default;

        void OnNavigatedTo(NavigationEventArgs const& args);
        void OnNavigatedFrom(NavigationEventArgs const& args);

        wil::single_threaded_rw_property<BaseProperties> BaseProperties;

        wil::single_threaded_property<SelectedItemsPropertiesViewModel> ViewModel;
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Properties, BasePropertiesPage);
