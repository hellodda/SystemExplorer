#pragma once

#include "Converters/StringFormatConverter.g.h"

namespace winrt::SystemExplorer::Converters::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::UI::Xaml::Interop;
    using namespace winrt::Microsoft::UI::Xaml::Data;
    using namespace winrt::Microsoft::UI::Xaml;


    struct StringFormatConverter : StringFormatConverterT<StringFormatConverter>
    {
        StringFormatConverter() = default;

        [[nodiscard]] IInspectable Convert(
            IInspectable const& value,
            TypeName const& type,
            IInspectable const& parameter,
            hstring const&
        );

        [[nodiscard]] IInspectable ConvertBack(
            IInspectable const&,
            TypeName const&,
            IInspectable const&,
            hstring const&
        );
    };
}
FACTORY(winrt::SystemExplorer::Converters, StringFormatConverter);
