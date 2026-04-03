#pragma once

#include "App.xaml.g.h"

namespace winrt::SystemExplorer::implementation
{
	using namespace winrt::Microsoft::UI::Xaml;

    struct App : AppT<App>
    {
        App();

        void OnLaunched(LaunchActivatedEventArgs const&);

		[[nodiscard]] static Window Window() { return window_; }

    private:
        static winrt::Microsoft::UI::Xaml::Window window_;
    };
}
