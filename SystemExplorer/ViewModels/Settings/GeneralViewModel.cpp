#include "pch.h"
#include "GeneralViewModel.h"
#if __has_include("ViewModels/Settings/GeneralViewModel.g.cpp")
#include "ViewModels/Settings/GeneralViewModel.g.cpp"
#endif

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
	IInspectable GeneralViewModel::SelectedAppLanguage() const noexcept
	{
		return SelectedAppLanguage_;
	}
	void GeneralViewModel::SelectedAppLanguage(IInspectable const& value) noexcept
	{
		if (SelectedAppLanguage_ != value)
		{
			SelectedAppLanguage_ = value;
		}
	}
}
