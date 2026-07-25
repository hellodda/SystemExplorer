#pragma once

#include "Selectors/ItemTemplateSelector.g.h"

#include <wil/cppwinrt_authoring.h>

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
}

namespace winrt::SystemExplorer::Xaml::Selectors::implementation
{
    struct ItemTemplateSelector : ItemTemplateSelectorT<ItemTemplateSelector>
    {
        ItemTemplateSelector() = default;

        wil::single_threaded_rw_property<hstring> Key;
        wil::single_threaded_rw_property<DataTemplate> PrimaryTemplate;
        wil::single_threaded_rw_property<DataTemplate> SecondaryTemplate;

        [[nodiscard]] DataTemplate SelectTemplateCore(IInspectable const& item);
        [[nodiscard]] DataTemplate SelectTemplateCore(IInspectable const& item, DependencyObject const& container);
    };
}

namespace winrt::SystemExplorer::Xaml::Selectors::factory_implementation
{
    struct ItemTemplateSelector : ItemTemplateSelectorT<ItemTemplateSelector, implementation::ItemTemplateSelector>
    {
    };
}
