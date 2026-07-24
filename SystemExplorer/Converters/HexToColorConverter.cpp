#include "pch.h"
#include "winrt_module_imports.h"
#include "HexToColorConverter.h"
#if __has_include("Converters/HexToColorConverter.g.cpp")
#include "Converters/HexToColorConverter.g.cpp"
#endif
#include <winrt/Windows.UI.h>

namespace winrt::SystemExplorer::Converters::implementation
{
	IInspectable HexToColorConverter::Convert(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&)
	{
		if (auto hexValue = value.try_as<hstring>())
		{
			try
			{
				return box_value(XamlToolkit::WinUI::Helpers::ColorHelper::ToColor(hexValue.value()));
			}
			catch (hresult_error const&)
			{
				return box_value(Windows::UI::Colors::Transparent());
			}
		}
		return nullptr;
	}
	IInspectable HexToColorConverter::ConvertBack(IInspectable const& value, TypeName const&, IInspectable const&, hstring const&)
	{
        Windows::UI::Color color;

        if (auto valueColor = value.try_as<Windows::UI::Color>())
        {
            color = *valueColor;
        }
        else if (auto valueBrush = value.try_as<Microsoft::UI::Xaml::Media::SolidColorBrush>())
        {
            color = valueBrush.Color();
        }
        else
        {
            return DependencyProperty::UnsetValue();
        }
        return box_value(XamlToolkit::WinUI::Helpers::ColorHelper::ToHex(color));
	}
}
