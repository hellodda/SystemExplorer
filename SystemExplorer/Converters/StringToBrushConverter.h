#pragma once

#include "Converters/StringToBrushConverter.g.h"
#include <factory.h>

namespace winrt::SystemExplorer::Converters::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::UI::Xaml::Interop;
    using namespace winrt::Microsoft::UI::Xaml::Data;
    using namespace winrt::Microsoft::UI::Xaml;

    struct StringToBrushConverter : StringToBrushConverterT<StringToBrushConverter>
    {
        StringToBrushConverter() = default;
        
        [[nodiscard]] IInspectable Convert(
            IInspectable const& value,
            TypeName const&,
            IInspectable const&,
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
FACTORY(winrt::SystemExplorer::Converters, StringToBrushConverter);
