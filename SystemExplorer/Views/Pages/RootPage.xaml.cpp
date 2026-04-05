#include "pch.h"
#include "RootPage.xaml.h"
#if __has_include("Views/Pages/RootPage.g.cpp")
#include "Views/Pages/RootPage.g.cpp"
#endif
#include <winrt/Windows.UI.Xaml.Interop.h>
#include "Settings/SettingsRootPage.xaml.h"
 
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
					pageTagStr = xaml_typename<SystemExplorer::Views::Pages::Settings::SettingsRootPage>().Name;
                
                TypeName typeName{ pageTagStr.value(), TypeKind::Custom };
                NavFrame().Navigate(typeName);
            }
        }
        co_return;
	}
}

