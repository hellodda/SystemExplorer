#pragma once

#include "ViewModels/RootViewModel.g.h"

#include "ViewModelBase.h"
#include "../Core/Data/Items/NavigationCatalogItem.h"

namespace winrt
{
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Media;
    using namespace winrt::Microsoft::UI::Xaml::Media::Imaging;

    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::SystemExplorer::Xaml;
    using namespace winrt::SystemExplorer::Core::Data;
}

namespace winrt::SystemExplorer::ViewModels::implementation
{
    struct RootViewModel : RootViewModelT<RootViewModel, ViewModelBase>
    {
        RootViewModel();

        wil::single_threaded_property<winrt::IAsyncRelayCommand> CreateLiveKernelMemoryDumpCommand = winrt::AsyncRelayCommandFactory::Make([this](auto&) -> winrt::IAsyncAction
        {
            co_await doCreateLiveKernelMemoryDumpAsync();
        });

        [[nodiscard]] winrt::Stretch AppThemeBackgroundImageFit() const noexcept;
        
        [[nodiscard]] winrt::VerticalAlignment AppThemeBackgroundImageVerticalAlignment() const noexcept;
        
        [[nodiscard]] winrt::HorizontalAlignment AppThemeBackgroundImageHorizontalAlignment() const noexcept;
        
        [[nodiscard]] winrt::ImageSource AppThemeBackgroundImageSource() const noexcept;
        
        [[nodiscard]] float AppThemeBackgroundImageOpacity() const noexcept;

        [[nodiscard]] winrt::IVector<winrt::IInspectable> TestItems()
        {
            static auto vector = []() -> auto
            {
                    auto catalogs = winrt::single_threaded_vector<winrt::IInspectable>();

                    Items::NavigationCatalogItem settingsCatalog;
                    Items::NavigationCatalogItem activitesCatalog;
                    settingsCatalog.Name(L"О Приложении");


                    winrt::Microsoft::UI::Xaml::Controls::FontIconSource f1;
                    f1.Glyph(L"\uE9F5");
                    f1.FontSize(16);
                    winrt::Microsoft::UI::Xaml::Controls::FontIconSource f2;
                    f2.Glyph(L"\uE9D2");

                    settingsCatalog.Icon(f1);
                    f2.FontSize(16);

                    settingsCatalog.Tags().Append(L"General");
                    settingsCatalog.Tags().Append(L"Additional");
                    settingsCatalog.Tags().Append(L"Settings");
                    /*settingsCatalog.Uri(winrt::Uri{ L"SystemExplorer://Settings/GeneralSettings/RemoteAccess" });*/
                    settingsCatalog.Uri(winrt::Uri{ L"system-explorer://SystemExplorer.Views.Pages/RemoteAccessPage" });
                    catalogs.Append(winrt::box_value(settingsCatalog));

                    activitesCatalog.Name(L"Производительность");
                    activitesCatalog.Uri(winrt::Uri{ L"system-explorer://SystemExplorer.Views.Pages.Settings/SettingsRootPage/ResourceUsagePage" });
                    activitesCatalog.Icon(f2);

                    catalogs.Append(winrt::box_value(activitesCatalog));
                    return catalogs;
            };

            return vector();
        }
    private:
        winrt::IAsyncAction doCreateLiveKernelMemoryDumpAsync();
    };
}
FACTORY(winrt::SystemExplorer::ViewModels, RootViewModel);