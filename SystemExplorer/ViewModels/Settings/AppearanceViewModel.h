#pragma once

#include "ViewModels/Settings/AppearanceViewModel.g.h"
#include <winrt/SystemExplorer.Xaml.Mvvm.Input.h>
#include <Core/Data/Factories/AppThemeResourceFactory.h>
#include <Core/Settings/UserSettings.h>
#include <Helpers/EnumHelper.h>
#include <macro.h>

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Windows::Foundation;

    using namespace winrt::SystemExplorer::Xaml::Mvvm::Input;
    using namespace winrt::SystemExplorer::Models::Items;
    using namespace winrt::SystemExplorer::Core;

    struct AppearanceViewModel : AppearanceViewModelT<AppearanceViewModel>, wil::notify_property_changed_base<AppearanceViewModel>
    {
        AppearanceViewModel();

        DECLARE_PROPERTY(hstring, AppThemeBackgroundColor);
        DECLARE_PROPERTY(AppThemeResourceItem, SelectedAppThemeResources);
        DECLARE_PROPERTY(int32_t, SelectedAppThemeIndex);
        DECLARE_PROPERTY(IInspectable, SelectedBackdropMaterial);
        DECLARE_PROPERTY(IInspectable, SelectedImageStretchType);
        DECLARE_PROPERTY(IInspectable, SelectedImageVerticalAlignmentType);
        DECLARE_PROPERTY(IInspectable, SelectedImageHorizontalAlignmentType);

        wil::single_threaded_property<IObservableVector<AppThemeResourceItem>> AppThemeResources =
            Data::Factories::AppThemeResourceFactory::AppThemeResources();

        wil::single_threaded_property<IVector<IInspectable>> BackdropMaterialTypes =
            single_threaded_vector<IInspectable>();

        wil::single_threaded_property<IVector<IInspectable>> ImageStretchTypes =
            single_threaded_vector<IInspectable>();

        wil::single_threaded_property<IVector<IInspectable>> ImageVerticalAlignmentTypes =
            single_threaded_vector<IInspectable>();

        wil::single_threaded_property<IVector<IInspectable>> ImageHorizontalAlignmentTypes =
            single_threaded_vector<IInspectable>();

        wil::single_threaded_notifying_property<int32_t> SelectedBackdropMaterialIndex;
        wil::single_threaded_notifying_property<int32_t> SelectedImageStretchTypeIndex;
        wil::single_threaded_notifying_property<int32_t> SelectedImageVerticalAlignmentTypeIndex;
        wil::single_threaded_notifying_property<int32_t> SelectedImageHorizontalAlignmentTypeIndex;

        wil::single_threaded_rw_property<int32_t> AppThemeBackgroundImageOpacity;
        wil::single_threaded_notifying_property<hstring> AppThemeBackgroundImageSource;
        
        wil::single_threaded_property<IAsyncRelayCommand> SelectImageCommand = AsyncRelayCommandFactory::Make([this](auto&) -> IAsyncAction {
            AppThemeBackgroundImageSource(L"C:\\Users\\user\\bublik.png");
            RaisePropertyChanged(L"AppThemeBackgroundImageSource");
            co_return;
        });
        wil::single_threaded_property<IAsyncRelayCommand> RemoveImageCommand = AsyncRelayCommandFactory::Make([this](auto&) -> IAsyncAction {
            co_return;
        });

    private:
        Core::Settings::IAppearanceSettings settings_ = Core::Settings::UserSettings::Instance().AppearanceSettings();

        void updateSelectedResource();
        void updateSelectedBackdropMaterial();
        void updateSelectedAppTheme();
        void updateSelectedImageStretch();
        void updateSelectedImageVerticalAlignmentType();
        void updateSelectedImageHorizontalAlignmentType();

        template<typename TSetting>
        void SetIndexFromSetting(Windows::Foundation::Collections::IVector<IInspectable> const& container, TSetting settingValue, auto indexSetter)
        {
            auto targetStr = winrt::SystemExplorer::Helpers::EnumHelper::Map(settingValue);
            auto index = IndexOf<IInspectable>(container, [&](IInspectable const& item) {
                return unbox_value_or<hstring>(item, L"") == targetStr;
                });
            (this->*indexSetter)(index >= 0 ? index : 0);
        }
    };
}

FACTORY(winrt::SystemExplorer::ViewModels::Settings, AppearanceViewModel);
