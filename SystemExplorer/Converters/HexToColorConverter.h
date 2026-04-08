#pragma once

#include "Converters/HexToColorConverter.g.h"

namespace winrt::SystemExplorer::Converters::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::UI::Xaml::Interop;
    using namespace winrt::Microsoft::UI::Xaml::Data;
    using namespace winrt::Microsoft::UI::Xaml;

    struct HexToColorConverter : HexToColorConverterT<HexToColorConverter>
    {
        HexToColorConverter() = default;

        [[nodiscard]] IInspectable Convert(
            IInspectable const& value,
            TypeName const&,
            IInspectable const&,
            hstring const&
        );

        [[nodiscard]] IInspectable ConvertBack(
            IInspectable const& value,
            TypeName const&,
            IInspectable const&,
            hstring const&
        );
    };
}

namespace winrt::SystemExplorer::Converters::factory_implementation
{
    struct HexToColorConverter : HexToColorConverterT<HexToColorConverter, implementation::HexToColorConverter>
    {
    };
}
