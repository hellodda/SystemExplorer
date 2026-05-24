#include "pch.h"
#include "ValueToSizeUnitConverter.h"
#if __has_include("Converters/ValueToSizeUnitConverter.g.cpp")
#include "Converters/ValueToSizeUnitConverter.g.cpp"
#endif

namespace winrt::SystemExplorer::Converters::implementation
{
    inline winrt::hstring FormatBytes(uint64_t bytes, std::wstring_view const& suffix)
    {
        static constexpr std::array units{ L" B", L" KB", L" MB", L" GB", L" TB" };

        double size = static_cast<double>(bytes);
        size_t unitIndex{ 0 };

        while (size >= 1024 && unitIndex < units.size() - 1)
        {
            size /= 1024;
            unitIndex++;
        }

        wchar_t buffer[28];
        
        auto length = swprintf_s(buffer, L"%.1f %ls%ls",
            size,
            units[unitIndex],
            suffix.data()
        );

        if (length > 0)
        {
            return winrt::hstring(buffer, static_cast<uint32_t>(length));
        }
        return L"";
    }

	IInspectable ValueToSizeUnitConverter::Convert(IInspectable const& value, TypeName const& type, IInspectable const& parameter, hstring const&)
	{
        if (!value) return winrt::box_value(L"");

        uint64_t bytes{ 0 };

        try 
        {
            auto pv = value.as<winrt::Windows::Foundation::IPropertyValue>();
            bytes = pv.GetUInt64();

            if (bytes == 0)
                return box_value(L"");
        }
        catch (...)
        {

        }

        std::wstring_view suffix{ L"" };
        if (parameter)
        {
            suffix = winrt::unbox_value<winrt::hstring>(parameter);
        }

        return winrt::box_value(FormatBytes(bytes, suffix));
	}
	IInspectable ValueToSizeUnitConverter::ConvertBack(IInspectable const&, TypeName const&, IInspectable const&, hstring const&)
	{
        throw hresult_not_implemented();
	}
}
