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
    AppearanceViewModel::AppearanceViewModel()
        : INIT_NOTIFYING_PROPERTY(SelectedBackdropMaterialIndex, 0),
          INIT_NOTIFYING_PROPERTY(SelectedImageStretchTypeIndex, 0),
          INIT_NOTIFYING_PROPERTY(SelectedImageVerticalAlignmentTypeIndex, 0),
          INIT_NOTIFYING_PROPERTY(SelectedImageHorizontalAlignmentTypeIndex, 0),
          INIT_NOTIFYING_PROPERTY(AppThemeBackgroundImageSource, L"")
    {
        updateSelectedResource();
        updateSelectedAppTheme();

        BackdropMaterialTypes.Append(box_value(StringsHelper::Static()));
        BackdropMaterialTypes.Append(box_value(StringsHelper::Acrylic()));
        BackdropMaterialTypes.Append(box_value(StringsHelper::ThinAcrylic()));
        BackdropMaterialTypes.Append(box_value(StringsHelper::Mica()));
        BackdropMaterialTypes.Append(box_value(StringsHelper::MicaAlt()));
        updateSelectedBackdropMaterial();

        ImageStretchTypes.Append(box_value(StringsHelper::None()));
        ImageStretchTypes.Append(box_value(StringsHelper::Fill()));
        ImageStretchTypes.Append(box_value(StringsHelper::Uniform()));
        ImageStretchTypes.Append(box_value(StringsHelper::UniformToFill()));
        updateSelectedImageStretch();

        ImageVerticalAlignmentTypes.Append(box_value(StringsHelper::Top()));
        ImageVerticalAlignmentTypes.Append(box_value(StringsHelper::Center()));
        ImageVerticalAlignmentTypes.Append(box_value(StringsHelper::Bottom()));
        updateSelectedImageVerticalAlignmentType();

        ImageHorizontalAlignmentTypes.Append(box_value(StringsHelper::Left()));
        ImageHorizontalAlignmentTypes.Append(box_value(StringsHelper::Center()));
        ImageHorizontalAlignmentTypes.Append(box_value(StringsHelper::Right()));
        updateSelectedImageHorizontalAlignmentType();

    }

    hstring AppearanceViewModel::AppThemeBackgroundColor() const
        noexcept(std::is_nothrow_copy_constructible_v<hstring>)
    {
        return settings_.AppThemeBackgroundColor();
    }

    void AppearanceViewModel::AppThemeBackgroundColor(hstring const& value)
        noexcept(std::is_nothrow_move_assignable_v<hstring>)
    {
        Core::Services::AppResourcesService::Instance().SetAppThemeBackgroundColor(ColorHelper::ToColor(value));
        settings_.AppThemeBackgroundColor(value);
        RaisePropertyChanged(L"AppThemeBackgroundColor");
    }

    AppThemeResourceItem AppearanceViewModel::SelectedAppThemeResources() const
        noexcept(std::is_nothrow_copy_constructible_v<AppThemeResourceItem>)
    {
        return SelectedAppThemeResources_;
    }

    void AppearanceViewModel::SelectedAppThemeResources(AppThemeResourceItem const& value)
        noexcept(std::is_nothrow_copy_constructible_v<AppThemeResourceItem>)
    {
        if (value != SelectedAppThemeResources_)
        {
            SelectedAppThemeResources_ = value;
            AppThemeBackgroundColor(value.BackgroundColor());
            RaisePropertyChanged(L"SelectedAppThemeResources");
        }
    }

    IInspectable AppearanceViewModel::SelectedBackdropMaterial() const
        noexcept(std::is_nothrow_copy_constructible_v<IInspectable>)
    {
        return SelectedBackdropMaterial_;
    }

    void AppearanceViewModel::SelectedBackdropMaterial(IInspectable const& value)
        noexcept(std::is_nothrow_copy_constructible_v<IInspectable>)
    {
        if (SelectedBackdropMaterial_ != value)
        {
            SelectedBackdropMaterial_ = value;
            settings_.BackdropMaterial(EnumHelper::Map<Data::Enums::BackdropMaterialType>(unbox_value<hstring>(value)));
            RaisePropertyChanged(L"SelectedBackdropMaterial");
        }
    }

    int32_t AppearanceViewModel::SelectedAppThemeIndex() const
        noexcept(std::is_nothrow_copy_constructible_v<int32_t>)
    {
        return SelectedAppThemeIndex_;
    }

    void AppearanceViewModel::SelectedAppThemeIndex(int32_t const& value)
        noexcept(std::is_nothrow_move_assignable_v<int32_t>)
    {
        static const std::array themes = { ElementTheme::Light, ElementTheme::Dark, ElementTheme::Default };
        if (value >= 0 && value < static_cast<int32_t>(themes.size()))
        {
            SelectedAppThemeIndex_ = value;
            settings_.ApplicationTheme(themes[value]);
            RaisePropertyChanged(L"SelectedAppThemeIndex");
        }
    }

    IInspectable AppearanceViewModel::SelectedImageStretchType() const noexcept(std::is_nothrow_copy_constructible_v<IInspectable>)
    {
        return SelectedImageStretchType_;
    }

    void AppearanceViewModel::SelectedImageStretchType(IInspectable const& value)
        noexcept(std::is_nothrow_copy_constructible_v<IInspectable>)
    {
        settings_.AppThemeBackgroundImageFit(EnumHelper::Map<Data::Enums::Stretch>(unbox_value<hstring>(value)));
    }

    IInspectable AppearanceViewModel::SelectedImageVerticalAlignmentType() const noexcept(std::is_nothrow_copy_constructible_v<IInspectable>)
    {
        return SelectedImageVerticalAlignmentType_;
    }

    void AppearanceViewModel::SelectedImageVerticalAlignmentType(IInspectable const& value)
        noexcept(std::is_nothrow_copy_constructible_v<IInspectable>)
    {
        settings_.AppThemeBackgroundImageVerticalAlignment(EnumHelper::Map<Data::Enums::VerticalAlignment>(unbox_value<hstring>(value)));
    }

    IInspectable AppearanceViewModel::SelectedImageHorizontalAlignmentType() const noexcept(std::is_nothrow_copy_constructible_v<IInspectable>)
    {
        return SelectedImageHorizontalAlignmentType_;
    }

    void AppearanceViewModel::SelectedImageHorizontalAlignmentType(IInspectable const& value)
        noexcept(std::is_nothrow_copy_constructible_v<IInspectable>)
    {
        settings_.AppThemeBackgroundImageHorizontalAlignment(EnumHelper::Map<Data::Enums::HorizontalAlignment>(unbox_value<hstring>(value)));
    }

    void AppearanceViewModel::updateSelectedResource()
    {
        const auto themeBackgroundColor = AppThemeBackgroundColor();
        const auto customName = StringsHelper::Custom();
        AppThemeResourceItem selected{ nullptr };

        for (auto const& p : AppThemeResources)
        {
            if (p.BackgroundColor() == themeBackgroundColor) {
                selected = p;
                break;
            }
        }

        if (!selected)
        {
            if (AppThemeResources.Size() > 0 && AppThemeResources.GetAt(AppThemeResources.Size() - 1).Name() == customName)
            {
                AppThemeResources.RemoveAtEnd();
            }
            selected = AppThemeResourceItem{};
            selected.BackgroundColor(themeBackgroundColor);
            selected.Name(customName);
            AppThemeResources.Append(selected);
        }
        SelectedAppThemeResources(selected);
    }

    void AppearanceViewModel::updateSelectedBackdropMaterial()
    {
        SetIndexFromSetting(BackdropMaterialTypes, settings_.BackdropMaterial(), &AppearanceViewModel::SelectedBackdropMaterialIndex);
    }

    void AppearanceViewModel::updateSelectedAppTheme()
    {
        auto theme = settings_.ApplicationTheme();
        int32_t index = (theme == ElementTheme::Light) ? 0 : (theme == ElementTheme::Dark ? 1 : 2);
        SelectedAppThemeIndex(index);
    }

    void AppearanceViewModel::updateSelectedImageStretch()
    {
        SetIndexFromSetting(ImageStretchTypes, settings_.AppThemeBackgroundImageFit(), &AppearanceViewModel::SelectedImageStretchTypeIndex);
    }

    void AppearanceViewModel::updateSelectedImageVerticalAlignmentType()
    {
        SetIndexFromSetting(ImageVerticalAlignmentTypes, settings_.AppThemeBackgroundImageVerticalAlignment(), &AppearanceViewModel::SelectedImageVerticalAlignmentTypeIndex);
    }

    void AppearanceViewModel::updateSelectedImageHorizontalAlignmentType()
    {
        SetIndexFromSetting(ImageHorizontalAlignmentTypes, settings_.AppThemeBackgroundImageHorizontalAlignment(), &AppearanceViewModel::SelectedImageHorizontalAlignmentTypeIndex);
    }
}
