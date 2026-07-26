#include "pch.h"
#include "winrt_module_imports.h"
#include "RootPage.xaml.h"
#if __has_include("Views/Pages/RootPage.g.cpp")
#include "Views/Pages/RootPage.g.cpp"
#endif
#include "Activities/ProcessesPage.xaml.h"
#include "Settings/SettingsRootPage.xaml.h"

namespace winrt::SystemExplorer::Views::Pages::implementation
{
    RootPage::RootPage()
    {
        InitializeComponent();

        NavFrame().Navigate(xaml_typename<Activities::ActivitiesRootPage>());
    }

	winrt::IAsyncAction RootPage::NavViewSelectionChanged(winrt::NavigationView const& sender, winrt::NavigationViewSelectionChangedEventArgs const& args)
	{
        if (auto item = args.SelectedItem())
        {
            if (auto pageTag = item.as<winrt::FrameworkElement>().Tag())
            {
				auto pageTagStr = pageTag.try_as<winrt::hstring>();

                if (pageTagStr == L"Settings")
					pageTagStr = xaml_typename<SystemExplorer::Views::Pages::Settings::SettingsRootPage>().Name;
                
                winrt::TypeName typeName{ pageTagStr.value(), winrt::TypeKind::Custom };
                NavFrame().Navigate(typeName);
            }
        }
        co_return;
	}
}

