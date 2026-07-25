#pragma once

#include "ViewModels/Settings/AppearanceViewModel.g.h"
#include "../ViewModelBase.h"

#include <winrt/Microsoft.Windows.Storage.Pickers.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/SystemExplorer.Xaml.Mvvm.Input.h>
#include <Core/Data/Factories/AppThemeResourceFactory.h>
#include <Helpers/EnumHelper.h>
#include <App.xaml.h>
#include <factory.h>
#include <property.h>

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    using namespace winrt::Microsoft::Windows::Storage::Pickers;
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Windows::Foundation;

    using namespace winrt::SystemExplorer::Xaml;
    using namespace winrt::SystemExplorer::Core::Data::Items;
    using namespace winrt::SystemExplorer::Core;

    struct AppearanceViewModel : AppearanceViewModelT<AppearanceViewModel, ViewModels::implementation::ViewModelBase>
    {
        AppearanceViewModel();

        wil::single_threaded_property<IAsyncRelayCommand> SelectImageCommand = AsyncRelayCommandFactory::Make([this](auto&) -> IAsyncAction {
            co_await doSelectImageAsync();
        });

        wil::single_threaded_property<IAsyncRelayCommand> RemoveImageCommand = AsyncRelayCommandFactory::Make([this](auto&) -> IAsyncAction {
            AppThemeBackgroundImageSource(L"");
            co_return;
        });

        DECLARE_PROPERTY(hstring, AppThemeBackgroundColor);
        DECLARE_PROPERTY(hstring, AppThemeBackgroundImageSource);
        DECLARE_PROPERTY(float, AppThemeBackgroundImageOpacity);

        DECLARE_ONLY_SETTER(int32_t, SelectedAppThemeIndex);
        DECLARE_ONLY_SETTER(AppThemeResourceItem, SelectedAppThemeResources);
        DECLARE_ONLY_SETTER(IInspectable, SelectedBackdropMaterial);
        DECLARE_ONLY_SETTER(IInspectable, SelectedImageStretchType);
        DECLARE_ONLY_SETTER(IInspectable, SelectedImageVerticalAlignmentType);
        DECLARE_ONLY_SETTER(IInspectable, SelectedImageHorizontalAlignmentType);

        wil::single_threaded_property<IObservableVector<AppThemeResourceItem>> AppThemeResources =
            Data::Factories::AppThemeResourceFactory::AppThemeResources();

        wil::single_threaded_property<IVector<IInspectable>> BackdropMaterialTypes = single_threaded_vector<IInspectable>();
        wil::single_threaded_property<IVector<IInspectable>> ImageStretchTypes = single_threaded_vector<IInspectable>();
        wil::single_threaded_property<IVector<IInspectable>> ImageVerticalAlignmentTypes = single_threaded_vector<IInspectable>();
        wil::single_threaded_property<IVector<IInspectable>> ImageHorizontalAlignmentTypes = single_threaded_vector<IInspectable>();

        wil::single_threaded_notifying_property<int32_t> SelectedBackdropMaterialIndex;
        wil::single_threaded_notifying_property<int32_t> SelectedImageStretchTypeIndex;
        wil::single_threaded_notifying_property<int32_t> SelectedImageVerticalAlignmentTypeIndex;
        wil::single_threaded_notifying_property<int32_t> SelectedImageHorizontalAlignmentTypeIndex;
    private:
        IAsyncAction doSelectImageAsync();

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