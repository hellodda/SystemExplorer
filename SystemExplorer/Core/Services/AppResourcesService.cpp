#include "pch.h"
#include "winrt_module_imports.h"
#include "AppResourcesService.h"

namespace winrt::SystemExplorer::Core::Services
{
	void AppResourcesService::SetAppThemeBackgroundColor(Color const& appThemeBackgroundColor)
	{
		auto brush = Application::Current()
			.Resources()
			.Lookup(winrt::box_value(L"App.Theme.BackgroundBrush"))
			.as<Media::SolidColorBrush>();

		brush.Color(appThemeBackgroundColor);
	}
}

