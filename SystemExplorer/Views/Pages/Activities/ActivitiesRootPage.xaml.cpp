#include "pch.h"
#include "winrt_module_imports.h"
#include "ActivitiesRootPage.xaml.h"
#if __has_include("Views/Pages/Activities/ActivitiesRootPage.g.cpp")
#include "Views/Pages/Activities/ActivitiesRootPage.g.cpp"
#endif
#include <Core/Settings/Settings.h>

namespace winrt::SystemExplorer::Views::Pages::Activities::implementation
{
    winrt::IAsyncAction ActivitiesRootPage::ActivitiesFrameLoaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& args)
    {
        if (ActivitiesFrame().Content() == nullptr)
        {
            auto startPage = Core::Settings::UserSettings::GeneralSettings.StartPage();
            auto startPageFull = L"SystemExplorer.Views.Pages.Activities." + startPage;

            ActivitiesFrame().Navigate(TypeName{ startPageFull, TypeKind::Metadata });
        }
        co_return;
    }

    winrt::IAsyncAction ActivitiesRootPage::ActivitiesNavViewSelectionChanged(winrt::NavigationView const& sender, winrt::NavigationViewSelectionChangedEventArgs const& args)
    {
        if (auto item = args.SelectedItem())
        {
            auto transitionInfo = winrt::SlideNavigationTransitionInfo{};
            transitionInfo.Effect(winrt::SlideNavigationTransitionEffect::FromRight);

            if (auto pageStr = item.as<winrt::Microsoft::UI::Xaml::FrameworkElement>().Tag())
            {
                TypeName typeName{ *pageStr.try_as<hstring>(), TypeKind::Custom };
                ActivitiesFrame().Navigate(typeName, nullptr, transitionInfo);
            }
        }
        co_return;
    }
}
