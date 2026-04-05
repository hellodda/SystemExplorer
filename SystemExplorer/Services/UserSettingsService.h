#pragma once
#include "AppearanceSettingsService.h"
#include "GeneralSettingsService.h"

namespace winrt::SystemExplorer::Services
{
	using namespace winrt::Windows::Foundation;

	struct UserSettingsService 
	{
		static UserSettingsService& Instance()
		{
			static UserSettingsService instance;
			return instance;
		}

	    bool ImportSettings(IInspectable const& import);
		IInspectable ExportSettings() const noexcept;
		
		std::shared_ptr<IAppearanceSettingsService> AppearanceSettingsService();
		std::shared_ptr<IGeneralSettingsService> GeneralSettingsService();

	private:
		UserSettingsService() = default;
	};
}


