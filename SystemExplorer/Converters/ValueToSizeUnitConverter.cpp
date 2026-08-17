#include "pch.h"
#include "winrt_module_imports.h"
#include "ValueToSizeUnitConverter.h"
#if __has_include("Converters/ValueToSizeUnitConverter.g.cpp")
#include "Converters/ValueToSizeUnitConverter.g.cpp"
#endif

namespace winrt::SystemExplorer::Converters::implementation
{
	winrt::IInspectable ValueToSizeUnitConverter::Convert(IInspectable const& value, TypeName const& type, IInspectable const& parameter, hstring const&)
	{
		auto get_value = [&]() -> std::optional<int64_t> 
		{
			if (auto v = value.try_as<int32_t>())  return *v;
			if (auto v = value.try_as<int64_t>())  return *v;
			if (auto v = value.try_as<uint32_t>()) return static_cast<int64_t>(*v);
			if (auto v = value.try_as<uint64_t>()) return static_cast<int64_t>(*v);
			return std::nullopt;
		};

		if (auto size = get_value())
		{
			return winrt::box_value(ToUnitSizeString(size.value()));
		}

		return winrt::box_value(winrt::hstring(L""));
	}
	
	winrt::IInspectable ValueToSizeUnitConverter::ConvertBack(IInspectable const&, TypeName const&, IInspectable const&, hstring const&)
	{
        throw hresult_not_implemented();
	}

	winrt::hstring ValueToSizeUnitConverter::ToUnitSizeString(size_t size)
	{
		static constexpr auto units = std::to_array<std::wstring_view>({ L"B", L"KB", L"MB", L"GB", L"TB", L"PB", L"EB" });
		double value = static_cast<double>(size);

		size_t index = 0;

		while (value >= 1024.0 && index < units.size() - 1)
		{
			value /= 1024.0;

			++index;
		}

		if (index == 0)
		{
			return winrt::format(L"{} {}", size, units[index]);
		}

		return winrt::format(L"{:.2f} {}", value, units[index]);
	}
}
