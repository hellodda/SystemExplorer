#include "pch.h"
#include "GeneralViewModel.h"
#if __has_include("ViewModels/Settings/GeneralViewModel.g.cpp")
#include "ViewModels/Settings/GeneralViewModel.g.cpp"
#endif
#include <Core/Services/AppLanguageService.h>

using namespace winrt::SystemExplorer::Core::Services;

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
	GeneralViewModel::GeneralViewModel() 
	  : INIT_NOTIFYING_PROPERTY(ShowRestartControl, false)
	{
		for (auto const& lang : AppLanguageService::Instance().SupportedLanguages())
			AppLanguages.Append(box_value(lang));
	}
	int32_t GeneralViewModel::SelectedAppLanguageIndex() const noexcept
	{
		return SelectedAppLanguageIndex_;
	}
	void GeneralViewModel::SelectedAppLanguageIndex(int32_t const& value) noexcept
	{
		if (AppLanguageService::Instance().TryChange(value))
		{
			SelectedAppLanguageIndex_ = value;
			RaisePropertyChanged(L"SelectedAppLanguageIndex");
			ShowRestartControl(true);
		}
	}
}
