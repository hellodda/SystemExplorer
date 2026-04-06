#include "pch.h"
#include "SettingsSerializer.h"
#if __has_include("Core/Serialization/SettingsSerializer.g.cpp")
#include "Core/Serialization/SettingsSerializer.g.cpp"
#endif
#include <winrt/Windows.Storage.h>

namespace winrt::SystemExplorer::Core::Serialization::implementation
{
	bool SettingsSerializer::CreateFile(hstring const& path)
	{
        return true;
	}
	hstring SettingsSerializer::ReadFromFile()
	{
        return hstring{ L"" };
	}
	bool SettingsSerializer::WriteToFile(hstring const& text)
	{
        return true;
	}
}
