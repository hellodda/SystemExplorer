#include "pch.h"
#include "winrt_module_imports.h"
#include "StringToBrushConverter.h"
#if __has_include("Converters/StringToBrushConverter.g.cpp")
#include "Converters/StringToBrushConverter.g.cpp"
#endif
#include <winrt/Windows.UI.h>

using namespace winrt::Microsoft::UI::Xaml::Media;
using namespace winrt::Windows::UI;

namespace winrt::SystemExplorer::Converters::implementation
{
	IInspectable StringToBrushConverter::Convert(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&)
	{
		if (auto strValue = value.try_as<hstring>())
		{
			try
			{
				return SolidColorBrush{ winrt::XamlToolkit::WinUI::Helpers::ColorHelper::ToColor(strValue.value()) };
			}
			catch (hresult_error const&)
			{
				return SolidColorBrush{ Colors::Transparent() };
			}
		}
		return nullptr;
	}
	IInspectable StringToBrushConverter::ConvertBack(IInspectable const&, TypeName const&, IInspectable const&, hstring const&)
	{
		throw hresult_not_implemented{};
	}
}
