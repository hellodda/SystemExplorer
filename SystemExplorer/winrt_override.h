#pragma once

namespace winrt
{
    inline winrt::hstring to_hstring(winrt::Windows::Foundation::IInspectable const& value)
    {
        if (!value)
            return L"";

        return winrt::unbox_value_or<winrt::hstring>(value, L"");
    }
}
