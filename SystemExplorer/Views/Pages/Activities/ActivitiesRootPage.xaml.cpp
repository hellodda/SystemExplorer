#include "pch.h"
#include "ActivitiesRootPage.xaml.h"
#if __has_include("Views/Pages/Activities/ActivitiesRootPage.g.cpp")
#include "Views/Pages/Activities/ActivitiesRootPage.g.cpp"
#endif
#include <Core/Settings/UserSettings.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Interop;


namespace winrt::SystemExplorer::Views::Pages::Activities::implementation
{
    IAsyncAction ActivitiesRootPage::ActivitiesFrameLoaded(IInspectable const& sender, RoutedEventArgs const& args)
    {
        if (ActivitiesFrame().Content() == nullptr)
        {
            auto startPage = Core::Settings::UserSettings::Instance().GeneralSettings().StartPage();
            auto startPageFull = L"SystemExplorer.Views.Pages.Activities." + startPage;

            ActivitiesFrame().Navigate(TypeName{ startPageFull, TypeKind::Metadata });
        }
        co_return;
    }
    IAsyncAction ActivitiesRootPage::ActivitiesNavViewSelectionChanged(NavigationView const& sender, NavigationViewSelectionChangedEventArgs const& args)
    {
        if (auto item = args.SelectedItem())
        {
            auto transitionInfo = winrt::Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo{};
            transitionInfo.Effect(winrt::Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromRight);

            if (auto pageStr = item.as<winrt::Microsoft::UI::Xaml::FrameworkElement>().Tag())
            {
                TypeName typeName{ *pageStr.try_as<hstring>(), TypeKind::Custom };
                ActivitiesFrame().Navigate(typeName, nullptr, transitionInfo);
            }
        }
        co_return;
    }
}
