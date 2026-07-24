#include "pch.h"
#include "winrt_module_imports.h"
#include "SettingsRootPage.xaml.h"
#if __has_include("Views/Pages/Settings/SettingsRootPage.g.cpp")
#include "Views/Pages/Settings/SettingsRootPage.g.cpp"
#endif

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    winrt::IAsyncAction SettingsRootPage::SettingsFrameLoaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& args)
    {
        if (SettingsFrame().Content() == nullptr)
        {
            SettingsNavView().SelectedItem(GeneralNavItem());
            SettingsFrame().Navigate(xaml_typename<Settings::GeneralPage>());
        }
        co_return;
    }

    winrt::IAsyncAction SettingsRootPage::SettingsNavViewSelectionChanged(winrt::NavigationView const& sender, winrt::NavigationViewSelectionChangedEventArgs const& args)
    {
        if (auto item = args.SelectedItem())
        {
            if (auto pageStr = item.as<winrt::Microsoft::UI::Xaml::FrameworkElement>().Tag())
            {
                winrt::TypeName typeName{ *pageStr.try_as<hstring>(), winrt::TypeKind::Custom };
                SettingsFrame().Navigate(typeName, nullptr);
            }
        }
        co_return;
    }
}
