#pragma once

namespace winrt::SystemExplorer::Services
{
	enum class BackdropMaterialType
	{
		Solid,
		Mica,
		MicaAlt,
		FlexibleMica,
		Acrylic,
		ThinAcrylic,
		FlexibleAcrylic
	};


	__interface IAppearanceSettingsService
	{
		winrt::hstring AppThemeMode();
		void AppThemeMode(winrt::hstring const& value);

		winrt::hstring AppThemeBackgroundColor();
		void AppThemeBackgroundColor(winrt::hstring const& value);

		BackdropMaterialType AppThemeBackdropMaterial();
		void AppThemeBackdropMaterial(BackdropMaterialType value);
	};

	struct AppearanceSettingsService : IAppearanceSettingsService
	{
	};
}

