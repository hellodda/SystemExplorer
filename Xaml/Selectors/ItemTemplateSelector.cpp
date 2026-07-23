#include "pch.h"
#include "ItemTemplateSelector.h"
#if __has_include("Selectors/ItemTemplateSelector.g.cpp")
#include "Selectors/ItemTemplateSelector.g.cpp"
#endif

namespace winrt::SystemExplorer::Xaml::Selectors::implementation
{
    DataTemplate implementation::ItemTemplateSelector::SelectTemplateCore(IInspectable const& item)
    {
        return SelectTemplateCore(item, nullptr);
    }
    DataTemplate ItemTemplateSelector::SelectTemplateCore(IInspectable const& item, DependencyObject const& container)
	{
        if (auto str = item.try_as<hstring>())
        {
            if (str.value() == Key)
            {
                return SecondaryTemplate;
            }
        }
        return PrimaryTemplate;
	}
}
