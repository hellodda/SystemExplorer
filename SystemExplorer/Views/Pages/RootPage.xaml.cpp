#include "pch.h"
#include "RootPage.xaml.h"
#if __has_include("Views/Pages/RootPage.g.cpp")
#include "Views/Pages/RootPage.g.cpp"
#endif
 
using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Windows::Foundation;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::SystemExplorer::Views::Pages::implementation
{
	IAsyncAction RootPage::NavViewSelectionChanged(NavigationView const& sender, NavigationViewSelectionChangedEventArgs const& args)
	{
        if (auto item = args.SelectedItem())
        {
            if (auto pageTag = item.as<winrt::Microsoft::UI::Xaml::FrameworkElement>().Tag())
            {
				auto pageTagStr = pageTag.try_as<hstring>();

                if (pageTagStr == L"Settings")
					pageTagStr = L"SystemExplorer.Views.Pages.Settings.SettingsRootPage";
                
                TypeName typeName{ pageTagStr.value(), TypeKind::Custom };
                NavFrame().Navigate(typeName);
            }
        }
        co_return;
	}
}

