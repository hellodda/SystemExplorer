#include "pch.h"
#include "TransparencyConverter.h"
#if __has_include("Converters/TransparencyConverter.g.cpp")
#include "Converters/TransparencyConverter.g.cpp"
#endif
#undef max

using namespace winrt::Microsoft::UI::Xaml::Media;

namespace winrt::SystemExplorer::Converters::implementation
{
	IInspectable TransparencyConverter::Convert(IInspectable const& value, TypeName const& type, IInspectable const& parameter, hstring const&)
	{
        if (!value) return winrt::box_value(0.1);

        auto numericValue{ 0.0 };

        if (auto propValue = value.try_as<winrt::Windows::Foundation::IPropertyValue>())
        {
            using namespace winrt::Windows::Foundation;
            switch (propValue.Type())
            {
            case PropertyType::Double:  numericValue = propValue.GetDouble(); break;
            case PropertyType::Single:  numericValue = propValue.GetSingle(); break;
            case PropertyType::Int32:   numericValue = propValue.GetInt32(); break;
            case PropertyType::UInt32:  numericValue = propValue.GetUInt32(); break;
            case PropertyType::Int64:   numericValue = propValue.GetInt64(); break;
            case PropertyType::UInt64:  numericValue = propValue.GetUInt64(); break;
            case PropertyType::Int16:   numericValue = propValue.GetInt16(); break;
            case PropertyType::UInt16:  numericValue = propValue.GetUInt16(); break;
            default: return winrt::box_value(0.1); 
            }
        }
        else
        {
            return winrt::box_value(0.1);
        }

        std::wstring_view mode = L"Auto";
        if (parameter)
        {
            if (auto paramStr = parameter.try_as<winrt::Windows::Foundation::IPropertyValue>())
            {
                if (paramStr.Type() == winrt::Windows::Foundation::PropertyType::String)
                {
                    mode = paramStr.GetString();
                }
            }
        }

        auto normalized{ 0.0 };

        if (mode == L"Percent")
            normalized = numericValue / 100.0;
        
        else if (mode == L"Log")
        {
            constexpr double MAX_LOG{ 9.0 };
            normalized = std::log10(std::max(0.0, numericValue) + 1.0) / MAX_LOG;
        }
        else if (mode == L"Normal")
        {
            normalized = numericValue;
        }
        else
        {
            if (numericValue <= 1.0)
                normalized = numericValue;                     
            else if (numericValue <= 100.0)
                normalized = numericValue / 100.0;            
            else
                normalized = std::log10(numericValue + 1.0) / 9.0; 
        }
        normalized = std::clamp(normalized, 0.0, 1.0);

        constexpr double MIN_OPACITY{ 0.1 };
        constexpr double MAX_OPACITY{ 0.3 };

        double resultOpacity = MIN_OPACITY + (normalized * (MAX_OPACITY - MIN_OPACITY));

        return winrt::box_value(std::clamp(resultOpacity, MIN_OPACITY, MAX_OPACITY));
	}
	IInspectable TransparencyConverter::ConvertBack(IInspectable const&, TypeName const&, IInspectable const&, hstring const&)
	{
		return IInspectable();
	}
}
