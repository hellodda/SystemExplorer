#include "pch.h"
#include "TextBlockTemplateSelector.h"
#if __has_include("Xaml/TextBlockTemplateSelector.g.cpp")
#include "Xaml/TextBlockTemplateSelector.g.cpp"
#endif

namespace winrt::SystemExplorer::Xaml::implementation
{
    winrt::DataTemplate TextBlockTemplateSelector::SelectTemplateCore(winrt::IInspectable const& item, winrt::DependencyObject const& container)
    {
        if (!item) return nullptr;

        if (auto string = item.try_as<winrt::hstring>())
        {
            return TextBlockTemplate;
        }
        else
        {
            return base_type::SelectTemplateCore(item, container);
        }
    }

    winrt::DataTemplate TextBlockTemplateSelector::SelectTemplateCore(winrt::IInspectable const& item)
    {
        return SelectTemplate(item, nullptr);
    }
}
