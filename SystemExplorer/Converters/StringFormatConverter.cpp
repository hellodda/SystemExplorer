#include "pch.h"
#include "StringFormatConverter.h"
#if __has_include("Converters/StringFormatConverter.g.cpp")
#include "Converters/StringFormatConverter.g.cpp"
#endif

namespace winrt::SystemExplorer::Converters::implementation
{
    IInspectable StringFormatConverter::Convert(
        IInspectable const& value,
        TypeName const&,
        IInspectable const& parameter,
        hstring const&)
    {
        if (!value) return nullptr;

        winrt::hstring replacementValue{ L"" };

        if (auto propValue = value.try_as<IPropertyValue>())
        {
            switch (propValue.Type())
            {
            case PropertyType::String:
                replacementValue = propValue.GetString();
                break;
            case PropertyType::Int32:
                replacementValue = winrt::to_hstring(propValue.GetInt32());
                break;
            case PropertyType::Double:
                replacementValue = winrt::to_hstring(propValue.GetDouble());
                break;
            case PropertyType::Single:
                replacementValue = winrt::to_hstring(propValue.GetSingle());
                break;
            case PropertyType::Int64:
                replacementValue = winrt::to_hstring(propValue.GetInt64());
                break;
            case PropertyType::UInt32:
                replacementValue = winrt::to_hstring(propValue.GetUInt32());
                break;
            case PropertyType::UInt64:
                replacementValue = winrt::to_hstring(propValue.GetUInt64());
                break;
            case PropertyType::Boolean:
                replacementValue = propValue.GetBoolean() ? L"True" : L"False";
                break;
            case PropertyType::Char16:
                replacementValue = winrt::hstring(std::wstring(1, propValue.GetChar16()));
                break;
            default:
                replacementValue = L""; 
                break;
            }
        }
        else if (auto stringable = value.try_as<winrt::Windows::Foundation::IStringable>())
        {
            replacementValue = stringable.ToString();
        }

        auto format = unbox_value_or<hstring>(parameter, L"");

        std::wstring replacement = replacementValue.c_str();
        std::wstring fmt = format.c_str();

        if (fmt._Starts_with(L"{}"))
        {
            fmt.erase(0, 2);
        }

        std::wstring toReplace = L"{0}";

        size_t pos = fmt.find(toReplace);

        while (pos != std::wstring::npos)
        {
            fmt.replace(pos, toReplace.length(), replacement);
            pos = fmt.find(toReplace, pos + replacement.length());
        }

        return box_value(fmt);
    }

    IInspectable StringFormatConverter::ConvertBack(
        IInspectable const&,
        TypeName const&,
        IInspectable const&,
        hstring const&)
    {
        throw hresult_not_implemented();
    }
}
