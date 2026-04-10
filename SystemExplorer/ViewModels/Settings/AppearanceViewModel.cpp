#include "pch.h"
#include "AppearanceViewModel.h"
#if __has_include("ViewModels/Settings/AppearanceViewModel.g.cpp")
#include "ViewModels/Settings/AppearanceViewModel.g.cpp"
#endif
#include <Helpers/StringsHelper.h>
#include <Helpers/EnumHelper.h>
#include <Helpers/Common.h>
#include <Core/Services/AppResourcesService.h>

using namespace winrt::Microsoft::UI::Xaml;

using namespace winrt::SystemExplorer::Helpers;
using namespace winrt::XamlToolkit::WinUI::Helpers;

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    const auto StaticBackdropMaterialName = StringsHelper::Static();
    const auto AcrylicBackdropMaterialName = StringsHelper::Acrylic();
    const auto ThinAcrylicBackdropMaterialName = StringsHelper::ThinAcrylic();
    const auto MicaBackdropMaterialName = StringsHelper::Mica();
    const auto MicaAltBackdropMaterialName = StringsHelper::MicaAlt();

    AppearanceViewModel::AppearanceViewModel()
    {
        updateSelectedResource();

        BackdropMaterialTypes.Append(box_value(StaticBackdropMaterialName));
        BackdropMaterialTypes.Append(box_value(AcrylicBackdropMaterialName));
        BackdropMaterialTypes.Append(box_value(ThinAcrylicBackdropMaterialName));
        BackdropMaterialTypes.Append(box_value(MicaBackdropMaterialName));
        BackdropMaterialTypes.Append(box_value(MicaAltBackdropMaterialName));

        updateSelectedBackdropMaterial();
        updateSelectedAppTheme();
    }

    hstring AppearanceViewModel::AppThemeBackgroundColor() const noexcept(std::is_nothrow_copy_constructible_v<hstring>)
    {
        return settings_.AppThemeBackgroundColor();
    }

    void AppearanceViewModel::AppThemeBackgroundColor(hstring const& value) noexcept(std::is_nothrow_move_assignable_v<hstring>)
    {
        Core::Services::AppResourcesService::Instance().SetAppThemeBackgroundColor(ColorHelper::ToColor(value));
        
        settings_.AppThemeBackgroundColor(value);

        RaisePropertyChanged(L"AppThemeBackgroundColor");
    }

    AppThemeResourceItem AppearanceViewModel::SelectedAppThemeResources() const noexcept(std::is_nothrow_copy_constructible_v<AppThemeResourceItem>)
    {
        return SelectedAppThemeResources_;
    }

    void AppearanceViewModel::SelectedAppThemeResources(AppThemeResourceItem const& value) noexcept(std::is_nothrow_copy_constructible_v<AppThemeResourceItem>)
    {
        if (value != SelectedAppThemeResources_)
        {
            SelectedAppThemeResources_ = value;

            AppThemeBackgroundColor(value.BackgroundColor());

            RaisePropertyChanged(L"SelectedAppThemeResources");
        }
    }

    IInspectable AppearanceViewModel::SelectedBackdropMaterial() const noexcept(std::is_nothrow_copy_constructible_v<IInspectable>)
    {
        return SelectedBackdropMaterial_;
    }

    void AppearanceViewModel::SelectedBackdropMaterial(IInspectable const& value) noexcept(std::is_nothrow_copy_constructible_v<IInspectable>)
    {
        if (SelectedBackdropMaterial_ != value)
        {
            SelectedBackdropMaterial_ = value;

            settings_.BackdropMaterial(EnumHelper::Map<Data::Enums::BackdropMaterialType>(unbox_value<hstring>(value)));

            RaisePropertyChanged(L"SelectedBackdropMaterial");
        }
    }


    int32_t AppearanceViewModel::SelectedAppThemeIndex() const noexcept(std::is_nothrow_copy_constructible_v<int32_t>)
    {
        return SelectedAppThemeIndex_;
    }

    void AppearanceViewModel::SelectedAppThemeIndex(int32_t const& value) noexcept(std::is_nothrow_move_assignable_v<int32_t>)
    {
        ElementTheme selectedTheme;

        switch (value)
        {
        case 0:
            selectedTheme = ElementTheme::Light;
            break;
        case 1:
            selectedTheme = ElementTheme::Dark;
            break;
        case 2:
            selectedTheme = ElementTheme::Default;
            break;
        default:
            break;
        }
        settings_.ApplicationTheme(selectedTheme);
    }

    void AppearanceViewModel::updateSelectedResource()
	{
        const auto themeBackgroundColor = AppThemeBackgroundColor(); 
        const auto CustomAppThemeResourceName = StringsHelper::Custom();

        AppThemeResourceItem selected{ nullptr };

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
                if (last.Name() == CustomAppThemeResourceName)
                {
                    AppThemeResources.RemoveAtEnd();
                }
            }
            auto item = AppThemeResourceItem{};
            item.BackgroundColor(themeBackgroundColor);
            item.Name(CustomAppThemeResourceName);

            AppThemeResources.Append(item);
            selected = item;                          
        }
        SelectedAppThemeResources(selected);
	}
    void AppearanceViewModel::updateSelectedBackdropMaterial()
    {
        auto currentMaterialStr = EnumHelper::Map(settings_.BackdropMaterial());

        auto index = IndexOf<IInspectable>(BackdropMaterialTypes(),
        [&](IInspectable const& item)
        {
            return unbox_value_or<hstring>(item, L"") == currentMaterialStr;
        });
        SelectedBackdropMaterialIndex(index >= 0 ? index : 0);
    }
    void implementation::AppearanceViewModel::updateSelectedAppTheme()
    {}
}
