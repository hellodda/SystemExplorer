#pragma once
#include "App.xaml.g.h"
#include "Models/AppModel.h"


namespace winrt::SystemExplorer::implementation
{
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::SystemExplorer::Models;
    using namespace winrt::WinUI3Package;

    struct App : AppT<App>
    {
        App();

        void OnLaunched(LaunchActivatedEventArgs const&);

        [[nodiscard]] static AppModel AppModel() { return appModel_; }
		[[nodiscard]] static WindowEx Window() { return window_; }
    private:
        static winrt::WinUI3Package::WindowEx window_;
        static winrt::SystemExplorer::Models::AppModel appModel_;
    };
}

namespace winrt::SystemExplorer
{
    using CurrentApplication = implementation::App;
}
