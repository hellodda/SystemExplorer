#pragma once

#include "ViewModels/AppearanceViewModel.g.h"
#include <Core/Data/Factories/AppThemeResourceFactory.h>
#include <Core/Settings/UserSettings.h>
#include <Core/Services/AppResourcesService.h>
#include <Helpers/StringsHelper.h>

namespace winrt::SystemExplorer::ViewModels::implementation
{
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Windows::Foundation;
    using namespace winrt::SystemExplorer::Helpers;

    const auto StaticBackdropMaterialName = StringsHelper::Static();
    const auto AcrylicBackdropMaterialName = StringsHelper::Acrylic();
    const auto ThinAcrylicBackdropMaterialName = StringsHelper::ThinAcrylic();
    const auto MicaBackdropMaterialName = StringsHelper::Mica();
    const auto MicaAltBackdropMaterialName = StringsHelper::MicaAlt();

    struct AppearanceViewModel : AppearanceViewModelT<AppearanceViewModel>, wil::notify_property_changed_base<AppearanceViewModel>
    {
        AppearanceViewModel();

        wil::single_threaded_property<IObservableVector<SystemExplorer::Models::Items::AppThemeResourceItem>> AppThemeResources =
            Core::Data::Factories::AppThemeResourceFactory::AppThemeResources();

        wil::single_threaded_property<IVector<IInspectable>> BackdropMaterialTypes =
            single_threaded_vector<IInspectable>();

        hstring AppThemeBackgroundColor() const noexcept
        {
           return Core::Settings::UserSettings::Instance().AppearanceSettings().AppThemeBackgroundColor();
        }
        void AppThemeBackgroundColor(hstring const& value)
        {
            auto color = XamlToolkit::WinUI::Helpers::ColorHelper::ToColor(value);

            Core::Services::AppResourcesService::Instance().SetAppThemeBackgroundColor(color);
            settings_.AppThemeBackgroundColor(value);

            RaisePropertyChanged(L"AppThemeBackgroundColor");
        }
        SystemExplorer::Models::Items::AppThemeResourceItem SelectedAppThemeResources() const noexcept
        {
            return selectedAppThemeResources_;
        }
        void SelectedAppThemeResources(SystemExplorer::Models::Items::AppThemeResourceItem const& value)
        {
            if (value)
            {
                AppThemeBackgroundColor(value.BackgroundColor());
                selectedAppThemeResources_ = value;
                RaisePropertyChanged(L"SelectedAppThemeResources");
                RaisePropertyChanged(L"AppThemeBackgroundColor");
            }
        }
        IInspectable SelectedBackdropMaterial() const noexcept
        {
            return selectedBackdropMaterial_;
        }
        void SelectedBackdropMaterial(IInspectable const& value)
        {
            selectedBackdropMaterial_ = value;
            
            auto typeStr = unbox_value<hstring>(value);

            settings_.BackdropMaterial(StringToEnum(typeStr));

            RaisePropertyChanged(L"SelectedBackdropMaterial");
        }
        wil::single_threaded_rw_property<int32_t> SelectedBackdropMaterialIndex;

    private:
        Core::Settings::IAppearanceSettings settings_ = Core::Settings::UserSettings::Instance().AppearanceSettings();

        IInspectable selectedBackdropMaterial_{};
        SystemExplorer::Models::Items::AppThemeResourceItem selectedAppThemeResources_{ L"Default", L"#00000000" };
    private:


        Core::Data::Enums::BackdropMaterialType StringToEnum(hstring const& value)
        {
            if (value == StaticBackdropMaterialName)
            {
                return Core::Data::Enums::BackdropMaterialType::Static;
            }
            else if (value == AcrylicBackdropMaterialName)
            {
                return Core::Data::Enums::BackdropMaterialType::Acrylic;
            }
            else if (value == ThinAcrylicBackdropMaterialName)
            {
                return Core::Data::Enums::BackdropMaterialType::ThinAcrylic;
            }
            else if (value == MicaBackdropMaterialName)
            {
                return Core::Data::Enums::BackdropMaterialType::Mica;
            }
            else if (value == MicaAltBackdropMaterialName)
            {
                return Core::Data::Enums::BackdropMaterialType::MicaAlt;
            }
            else
            {
                return Core::Data::Enums::BackdropMaterialType::Static;
            }
        }
        hstring EnumToString(Core::Data::Enums::BackdropMaterialType value)
        {
            switch (value)
            {
            case winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::Static:
                return StaticBackdropMaterialName;
            case winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::Mica:
                return MicaBackdropMaterialName;
            case winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::MicaAlt:
                return MicaAltBackdropMaterialName;
            case winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::Acrylic:
                return AcrylicBackdropMaterialName;
            case winrt::SystemExplorer::Core::Data::Enums::BackdropMaterialType::ThinAcrylic:
                return ThinAcrylicBackdropMaterialName;
            default:
                return StaticBackdropMaterialName;
            }
        }

        void updateSelectedResource();
        void updateSelectedBackdropMaterial();

        void selectBackgroundImage() {};
        void removeBackgroundImage() {};

    };
}

namespace winrt::SystemExplorer::ViewModels::factory_implementation
{
    struct AppearanceViewModel : AppearanceViewModelT<AppearanceViewModel, implementation::AppearanceViewModel>
    {
    };
}
