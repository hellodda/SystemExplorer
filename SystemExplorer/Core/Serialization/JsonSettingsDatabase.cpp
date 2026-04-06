#include "pch.h"
#include "JsonSettingsDatabase.h"
#if __has_include("Core/Serialization/JsonSettingsDatabase.g.cpp")
#include "Core/Serialization/JsonSettingsDatabase.g.cpp"
#endif

namespace winrt::SystemExplorer::Core::Serialization::implementation
{
	IInspectable JsonSettingsDatabase::GetValue(hstring const& key, IInspectable const& defaultValue)
	{
		return IInspectable();
	}
	bool JsonSettingsDatabase::SetValue(hstring const& key, IInspectable const& newValue)
	{
		return false;
	}
	bool JsonSettingsDatabase::RemoveKey(hstring const& key)
	{
		return false;
	}
	bool JsonSettingsDatabase::ImportSettings(IInspectable const& settings)
	{
		return false;
	}
	IInspectable JsonSettingsDatabase::ExportSettings()
	{
		return IInspectable();
	}
}
