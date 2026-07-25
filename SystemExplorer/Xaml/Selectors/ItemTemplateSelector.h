#pragma once

#include "Xaml/ItemTemplateSelector.g.h"

#ifdef __INTELLISENSE__
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
}

namespace winrt::SystemExplorer::Xaml::implementation
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

namespace winrt::SystemExplorer::Xaml::factory_implementation
{
    struct ItemTemplateSelector : ItemTemplateSelectorT<ItemTemplateSelector, implementation::ItemTemplateSelector>
    {
    };
}
