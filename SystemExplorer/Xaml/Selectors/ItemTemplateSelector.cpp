#include "pch.h"
#include "winrt_module_imports.h"
#include "ItemTemplateSelector.h"
#if __has_include("Xaml/ItemTemplateSelector.g.cpp")
#include "Xaml/ItemTemplateSelector.g.cpp"
#endif

namespace winrt::SystemExplorer::Xaml::implementation
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
