#include "pch.h"
#include "GeneralPage.xaml.h"
#include "winrt_module_imports.h"
#if __has_include("Views/Pages/Settings/GeneralPage.g.cpp")
#include "Views/Pages/Settings/GeneralPage.g.cpp"
#endif

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;


namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
	winrt::Windows::Foundation::IAsyncAction GeneralPage::TEST(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		this->Frame().Navigate(winrt::xaml_typename<winrt::SystemExplorer::Views::Pages::Settings::RemoteAccessPage>());
		co_return;
	}

	winrt::Windows::Foundation::IAsyncAction GeneralPage::TEST1(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		this->Frame().Navigate(winrt::xaml_typename<winrt::SystemExplorer::Views::Pages::Settings::EmsPage>());
		co_return;
	}
}
