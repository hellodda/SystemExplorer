#pragma once

#include "ViewModels/Settings/AboutViewModel.g.h"

#include <factory.h>

#include <property.h>

#ifdef __INTELLISENSE__
#include <winrt/Windows.System.h>
#endif

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::System;
    using namespace winrt::Microsoft::UI::Xaml::Input;
    using namespace winrt::SystemExplorer::Xaml;
}

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    struct AboutViewModel : AboutViewModelT<AboutViewModel>
    {
        AboutViewModel() = default;

        wil::single_threaded_property<winrt::IAsyncRelayCommand> LaunchUriCommand = AsyncRelayCommandFactory::Make([](winrt::IInspectable const& parameter) -> winrt::IAsyncAction {
            co_await winrt::Launcher::LaunchUriAsync(winrt::Uri{ parameter.as<winrt::hstring>() });
        });

        wil::single_threaded_property<winrt::IAsyncRelayCommand> OpenLogsCommand = AsyncRelayCommandFactory::Make([this](winrt::IInspectable const&) -> winrt::IAsyncAction {
            co_await doOpenLogsFolderAsync();
		});
    private:
        [[nodiscard]] winrt::IAsyncAction doOpenLogsFolderAsync();
    };
}
FACTORY(winrt::SystemExplorer::ViewModels::Settings, AboutViewModel);