#include "pch.h"
#include "winrt_module_imports.h"
#include "GeneralViewModel.h"
#if __has_include("ViewModels/Settings/GeneralViewModel.g.cpp")
#include "ViewModels/Settings/GeneralViewModel.g.cpp"
#endif
#include <Helpers/StringsHelper.h>
#include <Core/Services/AppLanguageService.h>
#include <Core/Settings/Settings.h>

#include <Views/Pages/Activities/ProcessesPage.xaml.h>

using namespace winrt::SystemExplorer::Core::Services;
using namespace winrt::SystemExplorer::Helpers;
using namespace winrt::SystemExplorer::Views;

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
	static constexpr auto ProcessesPageTypeName = winrt::impl::xaml_typename_name<Pages::Activities::ProcessesPage>::value();
	static constexpr auto ServicesPageTypeName = winrt::impl::xaml_typename_name<Pages::Activities::ServicesPage>::value();

	GeneralViewModel::GeneralViewModel() 
	  : INIT_NOTIFYING_PROPERTY(ShowRestartControl, false)
	{
		for (auto const& lang : AppLanguageService::SupportedLanguages())
			AppLanguages.Append(winrt::box_value(lang));

		StartPages.Append(winrt::ObjectPairItem{ winrt::box_value(StringsHelper::Processes()), winrt::box_value(ProcessesPageTypeName) });
		StartPages.Append(winrt::ObjectPairItem{ winrt::box_value(StringsHelper::Services()), winrt::box_value(ServicesPageTypeName) });
		StartPages.Append(winrt::ObjectPairItem{ winrt::box_value(StringsHelper::Performance()), nullptr });
	}

	void GeneralViewModel::SelectedStartPage(winrt::IInspectable value) noexcept
	{
		if (auto item = value.try_as<winrt::ObjectPairItem>())
		{
			Core::Settings::UserSettings::GeneralSettings.StartPage = winrt::unbox_value_or<winrt::hstring>(item.Second(), ProcessesPageTypeName);
		}
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
