#include "pch.h"
#include "winrt_module_imports.h"
#include "RemoteAccessPage.xaml.h"
#if __has_include("Views/Pages/Settings/RemoteAccessPage.g.cpp")
#include "Views/Pages/Settings/RemoteAccessPage.g.cpp"
#endif

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
	winrt::Windows::Foundation::IAsyncAction implementation::RemoteAccessPage::BtnClicked(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		auto dialog = RemoteComputerSetupDialog();

		// 2. Обязательно даем ему контекст окна (без этого будет серый экран)
		dialog.XamlRoot(this->XamlRoot());

		// 3. Показываем
		co_await dialog.ShowAsync();
	}
}
