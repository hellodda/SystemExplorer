#pragma once

namespace winrt::SystemExplorer::Core::Services
{
	using namespace winrt::Windows::UI;
	using namespace winrt::Microsoft::UI::Xaml;

	struct AppResourcesService final
	{
		static void SetAppThemeBackgroundColor(Color const& appThemeBackgroundColor);
	};
}
