#pragma once
#include "App.xaml.g.h"
#include "Core/Data/Models/AppModel.h"

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::WinUI3Package;
    using namespace winrt::Microsoft::UI;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::SystemExplorer::Core::Data::Models;
}

namespace winrt::SystemExplorer::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(winrt::LaunchActivatedEventArgs const&);

        [[nodiscard]] static winrt::WindowId GetCurrentWindowId() noexcept;
        [[nodiscard]] static AppModel AppModel() { return appModel_; }
		[[nodiscard]] static WindowEx Window() { return window_; }
    private:
        static inline winrt::WindowEx window_{ nullptr };
        static inline winrt::AppModel appModel_{};
    };
}

namespace winrt::SystemExplorer
{
    using CurrentApplication = implementation::App;
}

