#include "pch.h"
#include "winrt_module_imports.h"
#include "AppearanceViewModel.h"
#if __has_include("ViewModels/Settings/AppearanceViewModel.g.cpp")
#include "ViewModels/Settings/AppearanceViewModel.g.cpp"
#endif

#include <Core/Settings/Settings.h>
#include <Helpers/StringsHelper.h>
#include <Helpers/Common.h>

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

        BackdropMaterialTypes.Append(winrt::box_value(StringsHelper::Static()));
        BackdropMaterialTypes.Append(winrt::box_value(StringsHelper::Acrylic()));
        BackdropMaterialTypes.Append(winrt::box_value(StringsHelper::ThinAcrylic()));
        BackdropMaterialTypes.Append(winrt::box_value(StringsHelper::Mica()));
        BackdropMaterialTypes.Append(winrt::box_value(StringsHelper::MicaAlt()));
        updateSelectedBackdropMaterial();

        ImageStretchTypes.Append(winrt::box_value(StringsHelper::None()));
        ImageStretchTypes.Append(winrt::box_value(StringsHelper::Fill()));
        ImageStretchTypes.Append(winrt::box_value(StringsHelper::Uniform()));
        ImageStretchTypes.Append(winrt::box_value(StringsHelper::UniformToFill()));
        updateSelectedImageStretch();

        ImageVerticalAlignmentTypes.Append(winrt::box_value(StringsHelper::Top()));
        ImageVerticalAlignmentTypes.Append(winrt::box_value(StringsHelper::Center()));
        ImageVerticalAlignmentTypes.Append(winrt::box_value(StringsHelper::Bottom()));
        updateSelectedImageVerticalAlignmentType();

        ImageHorizontalAlignmentTypes.Append(winrt::box_value(StringsHelper::Left()));
        ImageHorizontalAlignmentTypes.Append(winrt::box_value(StringsHelper::Center()));
        ImageHorizontalAlignmentTypes.Append(winrt::box_value(StringsHelper::Right()));
        updateSelectedImageHorizontalAlignmentType();
    }

    void AppearanceViewModel::updateSelectedAppTheme()
    {
        auto theme = Core::Settings::UserSettings::AppearanceSettings.ApplicationTheme();
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

        FileOpenPicker picker{ SystemExplorer::CurrentApplication::GetCurrentWindowId() };
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
        //SetIndexFromSetting(ImageStretchTypes, Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageFit, &AppearanceViewModel::SelectedImageStretchTypeIndex);
    }

    void AppearanceViewModel::updateSelectedImageVerticalAlignmentType()
    {
        //SetIndexFromSetting(ImageVerticalAlignmentTypes, Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageVerticalAlignment, &AppearanceViewModel::SelectedImageVerticalAlignmentTypeIndex);
    }

    void AppearanceViewModel::updateSelectedImageHorizontalAlignmentType()
    {
        //SetIndexFromSetting(ImageHorizontalAlignmentTypes, Core::Settings::UserSettings::AppearanceSettings.AppThemeBackgroundImageHorizontalAlignment, &AppearanceViewModel::SelectedImageHorizontalAlignmentTypeIndex);
    }
}

