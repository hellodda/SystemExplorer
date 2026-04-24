#include "pch.h"
#include "SettingsRootPage.xaml.h"
#if __has_include("Views/Pages/Settings/SettingsRootPage.g.cpp")
#include "Views/Pages/Settings/SettingsRootPage.g.cpp"
#endif
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml::Interop;


namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    IAsyncAction SettingsRootPage::SettingsFrameLoaded(IInspectable const& sender, RoutedEventArgs const& args)
    {
        if (SettingsFrame().Content() == nullptr)
        {
            SettingsNavView().SelectedItem(GeneralNavItem());
            SettingsFrame().Navigate(xaml_typename<Settings::GeneralPage>());
        }
        co_return;
    }
    IAsyncAction SettingsRootPage::SettingsNavViewSelectionChanged(NavigationView const& sender, NavigationViewSelectionChangedEventArgs const& args)
    {
        if (auto item = args.SelectedItem())
        {
            auto transitionInfo = winrt::Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo{};
            transitionInfo.Effect(winrt::Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromRight);

            if (auto pageStr = item.as<winrt::Microsoft::UI::Xaml::FrameworkElement>().Tag())
            {
                TypeName typeName{ *pageStr.try_as<hstring>(), TypeKind::Custom };
                SettingsFrame().Navigate(typeName, nullptr, transitionInfo);
            }
        }
        co_return;
    }
}
