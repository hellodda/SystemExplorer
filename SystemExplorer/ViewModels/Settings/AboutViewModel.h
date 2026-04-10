#pragma once

#include "ViewModels/Settings/AboutViewModel.g.h"
#include <wil/cppwinrt_authoring.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Windows.System.h>
#include <winrt/SystemExplorer.Xaml.Mvvm.Input.h>

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

            if (auto link = parameter.try_as<hstring>())
            {
                LauncherOptions options;
                options.DisplayApplicationPicker(true);

                co_await Launcher::LaunchUriAsync(Uri{ link.value() }, options);
            }
        });
    };
}

FACTORY(winrt::SystemExplorer::ViewModels::Settings, AboutViewModel);