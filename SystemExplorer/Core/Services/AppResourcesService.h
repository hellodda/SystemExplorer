#pragma once
#include "ISingletonService.h"

namespace winrt::SystemExplorer::Core::Services
{
	using namespace winrt::Windows::UI;
	using namespace winrt::Microsoft::UI::Xaml;

	struct AppResourcesService
	{
		static void SetAppThemeBackgroundColor(Color const& appThemeBackgroundColor);
	};
}
