#pragma once

#include "ViewModels/Settings/AboutViewModel.g.h"
#include <wil/cppwinrt_authoring.h>
#include <winrt/Windows.System.h>
#include <winrt/SystemExplorer.Xaml.Mvvm.Input.h>
#include <factory.h>
#include <property.h>

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
	using namespace winrt::Microsoft::UI::Xaml::Input;
	using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::System;
    using namespace winrt::SystemExplorer::Xaml::Mvvm::Input;

    struct AboutViewModel : AboutViewModelT<AboutViewModel>
    {
        AboutViewModel() = default;

        wil::single_threaded_property<IAsyncRelayCommand> LaunchUriCommand = AsyncRelayCommandFactory::Make([](IInspectable const& parameter) -> IAsyncAction {
            co_await Launcher::LaunchUriAsync(Uri{ parameter.as<hstring>() });
        });

        wil::single_threaded_property<IAsyncRelayCommand> OpenLogsCommand = AsyncRelayCommandFactory::Make([this](IInspectable const&) -> IAsyncAction {
            co_await doOpenLogsFolderAsync();
		});
    private:
        IAsyncAction doOpenLogsFolderAsync();
    };
}
FACTORY(winrt::SystemExplorer::ViewModels::Settings, AboutViewModel);