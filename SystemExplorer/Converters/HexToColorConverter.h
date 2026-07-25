#pragma once

#include "Converters/HexToColorConverter.g.h"
#include <factory.h>

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::UI::Xaml::Interop;
    using namespace winrt::Microsoft::UI::Xaml::Data;
    using namespace winrt::Microsoft::UI::Xaml;
}

namespace winrt::SystemExplorer::Converters::implementation
{
    struct HexToColorConverter : HexToColorConverterT<HexToColorConverter>
    {
        HexToColorConverter() = default;

        [[nodiscard]] winrt::IInspectable Convert(
            winrt::IInspectable const& value,
            winrt::TypeName const&,
            winrt::IInspectable const&,
            winrt::hstring const&
        );

        [[nodiscard]] winrt::IInspectable ConvertBack(
            winrt::IInspectable const& value,
            winrt::TypeName const&,
            winrt::IInspectable const&,
            winrt::hstring const&
        );
    };
}
FACTORY(winrt::SystemExplorer::Converters, HexToColorConverter);
