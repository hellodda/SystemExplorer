#include "pch.h"
#include "winrt_module_imports.h"
#include "AppearanceViewModel.h"

#if __has_include("ViewModels/Settings/AppearanceViewModel.g.cpp")
#include "ViewModels/Settings/AppearanceViewModel.g.cpp"
#endif

#include <Core/Settings/Settings.h>

#include <Helpers/StringsHelper.h>
#include <Helpers/EnumHelper.h>
#include <Helpers/Common.h>
#include <Core/Services/AppResourcesService.h>

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::SystemExplorer::Helpers;
using namespace winrt::XamlToolkit::WinUI::Helpers;
using namespace winrt::SystemExplorer::Core::Data::Items;

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    AppearanceViewModel::AppearanceViewModel()
      : INIT_NOTIFYING_PROPERTY(SelectedBackdropMaterialIndex, 0),
        INIT_NOTIFYING_PROPERTY(SelectedImageStretchTypeIndex, 0),
        INIT_NOTIFYING_PROPERTY(SelectedImageVerticalAlignmentTypeIndex, 0),
        INIT_NOTIFYING_PROPERTY(SelectedImageHorizontalAlignmentTypeIndex, 0)
    {
        updateSelectedAppTheme();
        updateSelectedResource();

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

    void AppearanceViewModel::SelectedAppThemeIndex(int32_t const& value) noexcept
    {
        static const std::array themes = { ElementTheme::Light, ElementTheme::Dark, ElementTheme::Default };
        if (value >= 0 && value < static_cast<int32_t>(themes.size()))
        {
            SelectedAppThemeIndex_ = value;
            Core::Settings::UserSettings::AppearanceSettings.ApplicationTheme(themes[value]);
            RAISE_PROPERTY_CHANGED;
        }
    }

    void AppearanceViewModel::SelectedAppThemeResources(AppThemeResourceItem const& value) noexcept
    {
        if (value != SelectedAppThemeResources_)
        {
            SelectedAppThemeResources_ = value;
            AppThemeBackgroundColor(value.BackgroundColor());
            RAISE_PROPERTY_CHANGED;
        }
    }

    hstring AppearanceViewModel::AppThemeBackgroundColor() const noexcept
    {
        return Core::Settings::UserSettings::AppearanceSettings.ApplicationBackgroundColor();
    }

    void AppearanceViewModel::AppThemeBackgroundColor(hstring const& value) noexcept
    {
        Core::Services::AppResourcesService::Instance().SetAppThemeBackgroundColor(ColorHelper::ToColor(value));
        Core::Settings::UserSettings::AppearanceSettings.ApplicationBackgroundColor(value);
    }

    void AppearanceViewModel::SelectedBackdropMaterial(IInspectable const& value) noexcept
    {
        if (SelectedBackdropMaterial_ != value)
        {
            SelectedBackdropMaterial_ = value;
            Core::Settings::UserSettings::AppearanceSettings.BackdropMaterial(EnumHelper::Map<Data::Enums::BackdropMaterialType>(unbox_value<hstring>(value)));
            RAISE_PROPERTY_CHANGED;
        }
    }

    hstring AppearanceViewModel::AppThemeBackgroundImageSource() const noexcept
    {
        return Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageSource();
    }

    void AppearanceViewModel::AppThemeBackgroundImageSource(hstring const& value) noexcept
    {
        AppThemeBackgroundImageSource_ = value;
        Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageSource(value);
        RAISE_PROPERTY_CHANGED;
    }

    float AppearanceViewModel::AppThemeBackgroundImageOpacity() const noexcept
    {
        return Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageOpacity();
    }

    void AppearanceViewModel::AppThemeBackgroundImageOpacity(float const& value) noexcept
    {
        Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageOpacity(value);
        RAISE_PROPERTY_CHANGED;
    }

    void AppearanceViewModel::SelectedImageStretchType(IInspectable const& value) noexcept
    {
        Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageFit(EnumHelper::Map<Data::Enums::Stretch>(unbox_value<hstring>(value)));
    }

    void AppearanceViewModel::SelectedImageVerticalAlignmentType(IInspectable const& value) noexcept
    {
        Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageVerticalAlignment(EnumHelper::Map<Data::Enums::VerticalAlignment>(unbox_value<hstring>(value)));
    }

    void AppearanceViewModel::SelectedImageHorizontalAlignmentType(IInspectable const& value) noexcept
    {
        Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageHorizontalAlignment(EnumHelper::Map<Data::Enums::HorizontalAlignment>(unbox_value<hstring>(value)));
    }

    void AppearanceViewModel::updateSelectedAppTheme()
    {
        auto theme = Core::Settings::UserSettings::AppearanceSettings.ApplicationTheme;
        int32_t index = (theme == ElementTheme::Light) ? 0 : (theme == ElementTheme::Dark ? 1 : 2);
        SelectedAppThemeIndex(index);
    }

    IAsyncAction AppearanceViewModel::doSelectImageAsync()
    {
       /* auto extensions = std::vector<hstring>({
            StringsHelper::ImageFiles(), L"*.bmp;*.dib;*.jpg;*.jpeg;*.jpe;*.jfif;*.gif;*.tif;*.tiff;*.png;*.heic;*.hif;*.webp",
            StringsHelper::BitmapFiles(), L"*.bmp;*.dib",
            L"JPEG", L"*.jpg;*.jpeg;*.jpe;*.jfif",
            L"GIF", L"*.gif",
            L"TIFF", L"*.tif;*.tiff",
            L"PNG", L"*.png",
            L"HEIC", L"*.heic;*.hif",
            L"WEBP", L"*.webp"
        });*/

        FileOpenPicker picker{ SystemExplorer::implementation::App::Window().AppWindow().Id() };
        picker.FileTypeFilter().ReplaceAll({
            L".bmp", L".dib", L".jpg", L".jpeg", L".jpe", L".jfif",
            L".gif", L".tif", L".tiff", L".png", L".heic", L".hif", L".webp"
        });

        auto result = co_await picker.PickSingleFileAsync();

        if (result)
            AppThemeBackgroundImageSource(result.Path());
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
        //SetIndexFromSetting(BackdropMaterialTypes, Core::Settings::UserSettings::AppearanceSettings.Application, &AppearanceViewModel::SelectedBackdropMaterialIndex);
    }

    void AppearanceViewModel::updateSelectedImageStretch()
    {
        SetIndexFromSetting(ImageStretchTypes, Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageFit, &AppearanceViewModel::SelectedImageStretchTypeIndex);
    }

    void AppearanceViewModel::updateSelectedImageVerticalAlignmentType()
    {
        SetIndexFromSetting(ImageVerticalAlignmentTypes, Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageVerticalAlignment, &AppearanceViewModel::SelectedImageVerticalAlignmentTypeIndex);
    }

    void AppearanceViewModel::updateSelectedImageHorizontalAlignmentType()
    {
        SetIndexFromSetting(ImageHorizontalAlignmentTypes, Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageHorizontalAlignment, &AppearanceViewModel::SelectedImageHorizontalAlignmentTypeIndex);
    }
}