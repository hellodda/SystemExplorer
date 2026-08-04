#include "pch.h"
#include "winrt_module_imports.h"
#include "SettingsRootPage.xaml.h"
#if __has_include("Views/Pages/Settings/SettingsRootPage.g.cpp")
#include "Views/Pages/Settings/SettingsRootPage.g.cpp"
#endif
#include <core/data/items/NavigationBreadcrumbBarItem.h>
#include <core/eil/string.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    winrt::IAsyncAction SettingsRootPage::SettingsFrameLoaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& args)
    {
        if (NavigationFrame().Content() == nullptr)
        {
            SettingsNavView().SelectedItem(GeneralNavItem());
            NavigationFrame().Navigate(xaml_typename<Settings::GeneralPage>());
        }
        co_return;
    }

    winrt::IAsyncAction SettingsRootPage::SettingsNavViewSelectionChanged(winrt::NavigationView const& sender, winrt::NavigationViewSelectionChangedEventArgs const& args)
    {
        if (auto item = args.SelectedItem())
        {
            if (auto path = item.as<winrt::Microsoft::UI::Xaml::FrameworkElement>().Tag())
            {
                NavigateToUri(winrt::Uri{ path.as<winrt::hstring>() });
            }
        }
        co_return;
    }

    void SettingsRootPage::NavigateToUri(winrt::Uri const& uri)
    {
        auto ns = uri.Host();
        auto path = eil::split_path(uri.Path());
        auto target = path.back();

        auto targetTypeName = winrt::TypeName{ ns + L"." + target, winrt::TypeKind::Custom };

        NavigationFrame().Navigate(targetTypeName);

        auto vector = winrt::single_threaded_observable_vector<winrt::SystemExplorer::Core::Data::Items::NavigationBreadcrumbBarItem>();

        for (auto part : path)
        {
            auto pageType = winrt::TypeName{ winrt::hstring{ ns + L"." + part}, winrt::TypeKind::Custom};
            vector.Append(winrt::SystemExplorer::Core::Data::Items::NavigationBreadcrumbBarItem{ winrt::hstring{ part }, pageType });
        }
        NavigationBreadcrumbBar().ItemsSource(vector);
    }
}
