#pragma once
#include "ISingletonService.h"

namespace winrt::SystemExplorer::Core::Services
{
	using namespace winrt::Windows::UI;
	using namespace winrt::Microsoft::UI::Xaml;

	struct AppResourcesService : ISingletonService<AppResourcesService>
	{
		void SetAppThemeBackgroundColor(Color const& appThemeBackgroundColor);
	};
}
