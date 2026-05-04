#include "pch.h"
#include "GeneralViewModel.h"
#if __has_include("ViewModels/Settings/GeneralViewModel.g.cpp")
#include "ViewModels/Settings/GeneralViewModel.g.cpp"
#endif
#include <Helpers/StringsHelper.h>
#include <Core/Services/AppLanguageService.h>
#include <Core/Settings/UserSettings.h>

using namespace winrt::SystemExplorer::Core::Services;
using namespace winrt::SystemExplorer::Helpers;

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
	GeneralViewModel::GeneralViewModel() 
	  : INIT_NOTIFYING_PROPERTY(ShowRestartControl, false)
	{
		for (auto const& lang : AppLanguageService::Instance().SupportedLanguages())
			AppLanguages.Append(box_value(lang));

		StartPages.Append(box_value(StringsHelper::Processes()));
	}

	void GeneralViewModel::SelectedRealTimeUpdateSpeedIndex(int32_t const& value) noexcept
	{
		uint16_t updateSpeed{ 1500 };

		switch (value)
		{
		case 0: // high
			updateSpeed = 15;
			break;
		case 1: // def
			updateSpeed = 1000;
			break;
		case 2: // low
			updateSpeed = 2500;
			break;
		case 3: // suspend
			updateSpeed = 9999;
			break;
		default:
			updateSpeed = 1500;
			break;
		}
		Core::Settings::UserSettings::Instance().GeneralSettings().RealTimeUpdateSpeedMs(updateSpeed);
	}

	void GeneralViewModel::SelectedAppLanguageIndex(int32_t const& value) noexcept
	{
		if (AppLanguageService::Instance().TryChange(value))
		{
			SelectedAppLanguageIndex_ = value;
			ShowRestartControl(true);
			RAISE_PROPERTY_CHANGED;
		}
	}
}
