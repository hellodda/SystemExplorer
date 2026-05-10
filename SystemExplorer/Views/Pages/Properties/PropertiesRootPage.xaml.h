#pragma once
#include "BasePropertiesPage.h"
#include "Views/Pages/Properties/PropertiesRootPage.g.h"
#include <Views/Pages/Settings/AboutPage.xaml.h>

#include <winrt/Windows.UI.Xaml.Interop.h>
#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Properties::implementation
{
    using namespace winrt::Windows::UI::Xaml::Interop;
    using namespace winrt::Windows::Foundation;

    struct PropertiesRootPage : PropertiesRootPageT<PropertiesRootPage>
    {
        PropertiesRootPage()
        {
            this->ViewModel.ItemName(L"Test");
        }
        IAsyncAction MainFrameLoaded(const auto&, const auto&)
        {
            MainContentFrame().Navigate(xaml_typename<Properties::PropertiesGeneralPage>());
            co_return;
        }
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Properties, PropertiesRootPage);
