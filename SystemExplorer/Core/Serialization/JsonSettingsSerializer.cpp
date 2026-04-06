#include "pch.h"
#include "JsonSettingsSerializer.h"
#if __has_include("Core/Serialization/JsonSettingsSerializer.g.cpp")
#include "Core/Serialization/JsonSettingsSerializer.g.cpp"
#endif

namespace winrt::SystemExplorer::Core::Serialization::implementation
{
	hstring JsonSettingsSerializer::SerializeToJson(IInspectable const& obj) const noexcept
	{
		return hstring();
	}
	IInspectable JsonSettingsSerializer::DeserializeFromJson(hstring const& json) const noexcept
	{
		return IInspectable();
	}
}
