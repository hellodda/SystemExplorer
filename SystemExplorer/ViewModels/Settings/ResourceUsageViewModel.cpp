#include "pch.h"
#include "ResourceUsageViewModel.h"
#if __has_include("ViewModels/Settings/ResourceUsageViewModel.g.cpp")
#include "ViewModels/Settings/ResourceUsageViewModel.g.cpp"
#endif
#include <Core/Services/AppMemoryManager.h>
#include <Core/Settings/Settings.h>

using namespace winrt::SystemExplorer::Core;

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
	uint32_t ResourceUsageViewModel::InitialMemoryReserve() const noexcept
	{
		return InitialMemoryReserve_;
	}

	void ResourceUsageViewModel::InitialMemoryReserve(uint32_t value) noexcept
	{
		if (InitialMemoryReserve_ != value && value < 0)
		{
			InitialMemoryReserve_ = value;
			Core::Settings::UserSettings::ApplicationGlobalSettings().InitialMemoryReserve(value);
		}
	}

	uint32_t ResourceUsageViewModel::MemoryLimit() const noexcept
	{
		return MemoryLimit_;
	}

	void ResourceUsageViewModel::MemoryLimit(uint32_t value) noexcept
	{
		if (MemoryLimit_ != value && value < 0)
		{
			MemoryLimit_ = value;
			Core::Settings::UserSettings::ApplicationGlobalSettings().MemoryLimit(value);
		}
	}

	uint32_t ResourceUsageViewModel::PurgeDelay() const noexcept
	{
		return PurgeDelay_;
	}
	void ResourceUsageViewModel::PurgeDelay(uint32_t value) noexcept
	{
		if (PurgeDelay_ != value && value < 0)
		{
			PurgeDelay_ = value;
			Core::Settings::UserSettings::ApplicationGlobalSettings().PurgeDelay(value);
		}
	}

	bool ResourceUsageViewModel::UseLargeMemoryPages() const noexcept
	{
		return UseLargeMemoryPages_;
	}

	void ResourceUsageViewModel::UseLargeMemoryPages(bool value) noexcept
	{
		if (UseLargeMemoryPages_ != value)
		{
			UseLargeMemoryPages_ = value;
			Core::Settings::UserSettings::ApplicationGlobalSettings().UseLargeMemoryPages(value);
		}
	}

	void ResourceUsageViewModel::SelectedMemoryUsageModeIndex(int32_t value) noexcept
	{
		switch (value)
		{
		case 0: // default
			Services::AppMemoryManager::SetDefaultMemoryMode();
			break;
		case 1: // fast
			Services::AppMemoryManager::SetFastMemoryMode();
			break;
		case 2: // optimized
			Services::AppMemoryManager::SetOptimizedMemoryMode();
			break;
		case 3: // manual
		default:
			Services::AppMemoryManager::SetDefaultMemoryMode();
			break;
		}
	}
}
