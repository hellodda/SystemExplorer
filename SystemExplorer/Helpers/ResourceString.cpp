#include "pch.h"
#include "ResourceString.h"
#if __has_include("Helpers/ResourceString.g.cpp")
#include "Helpers/ResourceString.g.cpp"
#endif

namespace winrt::SystemExplorer::Helpers::implementation
{
	winrt::IInspectable implementation::ResourceString::ProvideValue()
	{
		if (Key.empty())
		{
			throw winrt::hresult_invalid_argument(L"Key property must be set on ResourceString before calling ProvideValue.");
		}
		return winrt::box_value(loader_.GetString(Key));
	}

	winrt::IInspectable implementation::ResourceString::ProvideValue([[maybe_unused]] winrt::IXamlServiceProvider const& provider)
	{
		return ProvideValue();
	}

	winrt::hstring implementation::ResourceString::LoadResource(winrt::hstring const& key)
	{
		return loader_.GetString(key);
	}
}
