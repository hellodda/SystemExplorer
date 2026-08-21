#include "pch.h"
#include "winrt_module_imports.h"
#include "GeneralViewModel.h"
#if __has_include("ViewModels/Settings/GeneralViewModel.g.cpp")
#include "ViewModels/Settings/GeneralViewModel.g.cpp"
#endif
#include <Helpers/StringsHelper.h>
#include <Core/Services/AppLanguageService.h>
#include <Core/Settings/Settings.h>


using namespace winrt::SystemExplorer::Core::Services;
using namespace winrt::SystemExplorer::Helpers;

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
	GeneralViewModel::GeneralViewModel() 
	  : INIT_NOTIFYING_PROPERTY(ShowRestartControl, false)
	{
		for (auto const& lang : AppLanguageService::SupportedLanguages())
			AppLanguages.Append(winrt::box_value(lang));

		//StartPages.Append(box_value(StringsHelper::Processes()));
	}

	void GeneralViewModel::SelectedRealTimeUpdateSpeedIndex(int32_t value) noexcept
	{
		uint16_t updateSpeed{ 750 };

		switch (value)
		{
		case 0: // high
			updateSpeed = 500;
			break;
		case 1: // def
			updateSpeed = 750;
			break;
		case 2: // low
			updateSpeed = 2500;
			break;
		case 3: // suspend
			updateSpeed = 10000;
			break;
		default:
			updateSpeed = 750;
			break;
		}
		Core::Settings::UserSettings::GeneralSettings.RealTimeUpdateSpeedMs(updateSpeed);
	}

	void GeneralViewModel::SelectedAppLanguageIndex(int32_t value) noexcept
	{
		if (AppLanguageService::TryChange(value))
		{
			SelectedAppLanguageIndex_ = value;
			ShowRestartControl(true);
		}
	}
}
