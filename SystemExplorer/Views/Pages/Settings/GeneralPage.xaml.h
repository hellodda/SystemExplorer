#pragma once

#include "Views/Pages/Settings/GeneralPage.g.h"

#include <ViewModels/Settings/GeneralViewModel.h>
#include <Xaml/Mvvm/AsyncRelayCommandFactory.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt
{
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Media;
    using namespace winrt::Microsoft::UI::Xaml::Controls;
}


namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    struct GeneralPage : GeneralPageT<GeneralPage>
    {
        GeneralPage() = default;

        wil::single_threaded_property<ViewModels::Settings::GeneralViewModel> ViewModel;


        wil::single_threaded_property<winrt::SystemExplorer::Xaml::IAsyncRelayCommand> NavigateToUriCommand = SystemExplorer::Xaml::AsyncRelayCommandFactory::Make([this](winrt::IInspectable const& parameter) -> winrt::IAsyncAction
        {
            winrt::DependencyObject current = this->try_as<winrt::DependencyObject>();
            winrt::SystemExplorer::Views::Pages::Settings::SettingsRootPage rootPage{ nullptr };

            while (current)
            {
                rootPage = current.try_as<winrt::SystemExplorer::Views::Pages::Settings::SettingsRootPage>();
                if (rootPage)
                {
                    break;
                }
                current = winrt::VisualTreeHelper::GetParent(current);
            }
            if (rootPage)
            {
                rootPage.NavigateToUri(winrt::Uri{ parameter.as<winrt::hstring>().c_str() });
            }

            co_return;
        });
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Settings, GeneralPage);