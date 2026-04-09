#include "pch.h"
#include "AppearanceViewModel.h"
#if __has_include("ViewModels/AppearanceViewModel.g.cpp")
#include "ViewModels/AppearanceViewModel.g.cpp"
#endif

namespace winrt::SystemExplorer::ViewModels::implementation
{
    AppearanceViewModel::AppearanceViewModel()
    {
        updateSelectedResource();

        BackdropMaterialTypes.Append(box_value(StaticBackdropMaterialName));
        BackdropMaterialTypes.Append(box_value(AcrylicBackdropMaterialName));
        BackdropMaterialTypes.Append(box_value(ThinAcrylicBackdropMaterialName));
        BackdropMaterialTypes.Append(box_value(MicaBackdropMaterialName));
        BackdropMaterialTypes.Append(box_value(MicaAltBackdropMaterialName));

        updateSelectedBackdropMaterial();
    }

	void AppearanceViewModel::updateSelectedResource()
	{
        const auto themeBackgroundColor = AppThemeBackgroundColor(); 
        const auto CusomString = StringsHelper::Custom();

        SystemExplorer::Models::Items::AppThemeResourceItem selected{ nullptr };

        for (auto const& p : AppThemeResources)
        {
            if (p.BackgroundColor() == themeBackgroundColor)
            {
                selected = p;
                break;
            }
        }
        if (!selected)
        {
            if (AppThemeResources.Size() > 0)
            {
                auto last = AppThemeResources.GetAt(AppThemeResources.Size() - 1);
                if (last.Name() == CusomString)
                {
                    AppThemeResources.RemoveAtEnd();
                }
            }
            auto item = SystemExplorer::Models::Items::AppThemeResourceItem();
            item.BackgroundColor(themeBackgroundColor);
            item.Name(CusomString);

            AppThemeResources.Append(item);
            selected = item;                          
        }
        SelectedAppThemeResources(selected);
	}
    void AppearanceViewModel::updateSelectedBackdropMaterial()
    {
        auto currentMaterialStr = EnumToString(settings_.BackdropMaterial());

        auto materials = BackdropMaterialTypes();
        auto size = materials.Size();
        auto foundIndex{ 0 };

        for (uint32_t i = 0; i < size; ++i)
        {
            auto itemStr = winrt::unbox_value_or<hstring>(materials.GetAt(i), L"");

            if (itemStr == currentMaterialStr)
            {
                foundIndex = static_cast<int32_t>(i);
                break;
            }
        }
        SelectedBackdropMaterialIndex(foundIndex);
    }
}
