#pragma once
#include "App.xaml.g.h"
#include "Core/System/ProviderThread.h"
#include "Core/Data/Models/AppModel.h"

namespace winrt::SystemExplorer::implementation
{
    using namespace winrt::WinUI3Package;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::SystemExplorer::Core::Data::Models;

    struct App : AppT<App>
    {
        App();

        void OnLaunched(LaunchActivatedEventArgs const&);

        [[nodiscard]] static AppModel AppModel() { return appModel_; }
		[[nodiscard]] static WindowEx Window() { return window_; }
    private:
        static winrt::WinUI3Package::WindowEx window_;
        static winrt::SystemExplorer::Core::Data::Models::AppModel appModel_;
    };
}

namespace winrt::SystemExplorer
{
    using CurrentApplication = implementation::App;
}

