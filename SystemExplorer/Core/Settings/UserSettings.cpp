#include "pch.h"
#include "UserSettings.h"
#if __has_include("Core/Settings/UserSettings.g.cpp")
#include "Core/Settings/UserSettings.g.cpp"
#endif

namespace winrt::SystemExplorer::Core::Settings::implementation
{
	Settings::UserSettings implementation::UserSettings::Instance()
	{
		static auto instance = winrt::make<implementation::UserSettings>();
		return instance;
	}
}
