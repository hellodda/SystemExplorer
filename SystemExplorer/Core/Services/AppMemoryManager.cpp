#include "pch.h"
#include "AppMemoryManager.h"

#include <mimalloc.h>
#include <Core/Settings/Settings.h>

namespace winrt::SystemExplorer::Core::Services
{
	void AppMemoryManager::LoadMemoryOptions()
	{
		try
		{
			mi_option_set(mi_option_arena_reserve, Settings::UserSettings::ApplicationGlobalSettings().InitialMemoryReserve());
			mi_option_set(mi_option_limit_os_alloc, Settings::UserSettings::ApplicationGlobalSettings().MemoryLimit());
			mi_option_set(mi_option_purge_delay, Settings::UserSettings::ApplicationGlobalSettings().PurgeDelay());
			mi_option_set(mi_option_allow_large_os_pages, Settings::UserSettings::ApplicationGlobalSettings().UseLargeMemoryPages() ? 1 : 0);
		}
		catch (std::exception const& ex)
		{
			SetDefaultMemoryMode();
		}
		CATCH_LOG()
		
		Settings::UserSettings::ApplicationGlobalSettings().SettingChanged->operator()([](std::wstring_view key, winrt::Windows::Foundation::IInspectable const& value)
		{
			if (key == L"InitialMemoryReserve")
			{
				mi_option_set(mi_option_arena_reserve, winrt::unbox_value<uint32_t>(value));
			}
			else if (key == L"MemoryLimit")
			{
				mi_option_set(mi_option_limit_os_alloc, winrt::unbox_value<uint32_t>(value));
			}
			else if (key == L"PurgeDelay")
			{
				mi_option_set(mi_option_purge_delay, winrt::unbox_value<uint32_t>(value));
			}
			else if (key == L"UseLargeMemoryPages")
			{
				mi_option_set(mi_option_allow_large_os_pages, winrt::unbox_value<bool>(value) ? 1 : 0);
			}
		});
	}

	void AppMemoryManager::SetDefaultMemoryMode()
	{
		mi_option_set(mi_option_purge_delay, 0);
		mi_option_set(mi_option_reset_delay, 0);
		mi_option_set(mi_option_arena_reserve, 0);
	}

	void AppMemoryManager::SetFastMemoryMode()
	{
		mi_option_set(mi_option_purge_delay, 10);
		mi_option_set(mi_option_arena_reserve, 32768);
		mi_option_set(mi_option_allow_large_os_pages, 1);
	}

	void AppMemoryManager::SetOptimizedMemoryMode()
	{

	}
}