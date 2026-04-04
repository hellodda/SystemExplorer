#pragma once

#include "ViewModels/Pages/Settings/AboutViewModel.g.h"
#include <wil/cppwinrt_authoring.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Windows.System.h>

namespace winrt::SystemExplorer::ViewModels::Pages::Settings::implementation
{
	using namespace winrt::Microsoft::UI::Xaml::Input;
	using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::System;

    struct AboutViewModel : AboutViewModelT<AboutViewModel>
    {
        AboutViewModel() = default;

        wil::single_threaded_property<SystemExplorer::Xaml::IAsyncRelayCommand> LaunchUriCommand = SystemExplorer::Xaml::AsyncRelayCommand([](IInspectable const& parameter) -> IAsyncAction {
            co_await Launcher::LaunchUriAsync(Uri{ parameter.try_as<hstring>().value() });
        });
    };
}

namespace winrt::SystemExplorer::ViewModels::Pages::Settings::factory_implementation
{
    struct AboutViewModel : AboutViewModelT<AboutViewModel, implementation::AboutViewModel>
    {
    };
}
