#include "pch.h"
#include "UserSettingsService.h"

namespace winrt::SystemExplorer::Services
{
	bool UserSettingsService::ImportSettings(IInspectable const& import)
	{
		return false;
	}
	IInspectable UserSettingsService::ExportSettings() const noexcept
	{
		return IInspectable();
	}

	std::shared_ptr<IAppearanceSettingsService> winrt::SystemExplorer::Services::UserSettingsService::AppearanceSettingsService()
	{
		throw winrt::hresult_error{};
	}
	std::shared_ptr<IGeneralSettingsService> UserSettingsService::GeneralSettingsService()
	{
		throw winrt::hresult_error{};
	}
}