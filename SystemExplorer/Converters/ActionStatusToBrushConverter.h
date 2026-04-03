#pragma once

#include "Converters/ActionStatusToBrushConverter.g.h"

namespace winrt::SystemExplorer::Converters::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::UI::Xaml::Interop;
    using namespace winrt::Microsoft::UI::Xaml::Data;
    using namespace winrt::Microsoft::UI::Xaml;

    struct ActionStatusToBrushConverter : ActionStatusToBrushConverterT<ActionStatusToBrushConverter>
    {
        ActionStatusToBrushConverter() = default;

        [[nodiscard]] IInspectable Convert(
            _In_ IInspectable const& value,
            _In_ TypeName const&,
            _In_ IInspectable const&,
            _In_ hstring const&
        );

        [[nodiscard]] IInspectable ConvertBack(
            _In_ IInspectable const&,
            _In_ TypeName const&,
            _In_ IInspectable const&,
            _In_ hstring const&
        );
    };
}

namespace winrt::SystemExplorer::Converters::factory_implementation
{
    struct ActionStatusToBrushConverter : ActionStatusToBrushConverterT<ActionStatusToBrushConverter, implementation::ActionStatusToBrushConverter>
    {
    };
}
