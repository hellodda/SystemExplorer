#pragma once

#include "Converters/ValueToSizeUnitConverter.g.h"
#include <factory.h>

namespace winrt::SystemExplorer::Converters::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::UI::Xaml::Interop;
    using namespace winrt::Microsoft::UI::Xaml::Data;
    using namespace winrt::Microsoft::UI::Xaml;

    struct ValueToSizeUnitConverter : ValueToSizeUnitConverterT<ValueToSizeUnitConverter>
    {
        ValueToSizeUnitConverter() = default;

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

    private:
        
        winrt::hstring ToUnitSizeString(size_t size);
    };
}
FACTORY(winrt::SystemExplorer::Converters, ValueToSizeUnitConverter);