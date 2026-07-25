#include "pch.h"
#include "winrt_module_imports.h"
#include "HexToColorConverter.h"
#if __has_include("Converters/HexToColorConverter.g.cpp")
#include "Converters/HexToColorConverter.g.cpp"
#endif

namespace winrt::SystemExplorer::Converters::implementation
{
	winrt::IInspectable HexToColorConverter::Convert(winrt::IInspectable const& value, winrt::TypeName const&, winrt::IInspectable const&, winrt::hstring const&)
	{
		if (auto hexValue = value.try_as<winrt::hstring>())
		{
			try
			{
				return winrt::box_value(XamlToolkit::WinUI::Helpers::ColorHelper::ToColor(hexValue.value()));
			}
			catch (hresult_error const&)
			{
				return winrt::box_value(Windows::UI::Colors::Transparent());
			}
		}
		return nullptr;
	}

	winrt::IInspectable HexToColorConverter::ConvertBack(winrt::IInspectable const& value, winrt::TypeName const&, winrt::IInspectable const&, winrt::hstring const&)
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
            return winrt::DependencyProperty::UnsetValue();
        }
        return winrt::box_value(XamlToolkit::WinUI::Helpers::ColorHelper::ToHex(color));
	}
}
