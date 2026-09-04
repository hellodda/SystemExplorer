#pragma once

#include "Xaml/TextBlockTemplateSelector.g.h"

#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
}

namespace winrt::SystemExplorer::Xaml::implementation
{
    struct TextBlockTemplateSelector : TextBlockTemplateSelectorT<TextBlockTemplateSelector>
    {
        TextBlockTemplateSelector() = default;

        [[nodiscard]] winrt::DataTemplate SelectTemplateCore(winrt::IInspectable const& item, winrt::DependencyObject const& container);
        [[nodiscard]] winrt::DataTemplate SelectTemplateCore(winrt::IInspectable const& item);

        wil::single_threaded_rw_property<winrt::DataTemplate> TextBlockTemplate;
    };
}

namespace winrt::SystemExplorer::Xaml::factory_implementation
{
    struct TextBlockTemplateSelector : TextBlockTemplateSelectorT<TextBlockTemplateSelector, implementation::TextBlockTemplateSelector>
    {
    };
}
