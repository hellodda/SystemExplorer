#include "pch.h"
#include "winrt_module_imports.h"
#include "GeneralViewModel.h"

#include <Core/Settings/Settings.h>

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
	void GeneralViewModel::AppWindowAlwaysOnTop(bool value) noexcept
	{
		if (value != AppWindowAlwaysOnTop_)
		{
			Core::Settings::UserSettings::GeneralSettings.AppWindowAlwaysOnTop = value;
		}
	}

	bool GeneralViewModel::AppWindowAlwaysOnTop() const noexcept
	{
		return Core::Settings::UserSettings::GeneralSettings.AppWindowAlwaysOnTop();
	}

	void GeneralViewModel::AppWindowMinimizeOnUse(bool value) noexcept
	{
		if (value != AppWindowMinimizeOnUse_)
		{
			Core::Settings::UserSettings::GeneralSettings.AppWindowMinimizeOnUse = value;
		}
	}

	bool GeneralViewModel::AppWindowMinimizeOnUse() const noexcept
	{
		return Core::Settings::UserSettings::GeneralSettings.AppWindowMinimizeOnUse();
	}

	void GeneralViewModel::AppWindowHideWhenMinimized(bool value) noexcept
	{
		if (value != AppWindowHideWhenMinimized_)
		{
			Core::Settings::UserSettings::GeneralSettings.AppWindowHideWhenMinimized = value;
		}
	}

	bool GeneralViewModel::AppWindowHideWhenMinimized() const noexcept
	{
		return Core::Settings::UserSettings::GeneralSettings.AppWindowHideWhenMinimized();
	}
}