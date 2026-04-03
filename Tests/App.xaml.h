#pragma once

#include "App.xaml.g.h"

namespace winrt::SystemExplorer::Tests::implementation
{
    struct App : AppT<App>
    {
        App(PWSTR argv);

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);

    private:
        winrt::hstring m_args;
    };
}
