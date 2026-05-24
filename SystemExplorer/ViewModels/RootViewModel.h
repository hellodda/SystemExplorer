#pragma once

#include "ViewModels/RootViewModel.g.h"
#include "ViewModelBase.h"
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <Core/Settings/AppearanceSettings.h>
#include <Core/Settings/UserSettings.h>

#include <winrt/SystemExplorer.Xaml.Mvvm.Input.h>

namespace winrt::SystemExplorer::ViewModels::implementation
{
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Media;
    using namespace winrt::Microsoft::UI::Xaml::Media::Imaging;

    using namespace winrt::Windows::Foundation;

    using namespace winrt::SystemExplorer::Core;
    using namespace winrt::SystemExplorer::Xaml::Mvvm::Input;

    struct RootViewModel : RootViewModelT<RootViewModel, ViewModelBase>
    {
        RootViewModel()
        {
            settings_.SettingChanged([this](auto& sender, auto& args) {
                onSettingChanged(sender, args);
            });
        }

        wil::single_threaded_property<IAsyncRelayCommand> CreateLiveKernelMemoryDumpCommand = AsyncRelayCommandFactory::Make([this](auto&) -> IAsyncAction {
            co_await doCreateLiveKernelMemoryDumpAsync();
        });


        Stretch AppThemeBackgroundImageFit() const noexcept;
        VerticalAlignment AppThemeBackgroundImageVerticalAlignment() const noexcept;
        HorizontalAlignment AppThemeBackgroundImageHorizontalAlignment() const noexcept;
        ImageSource AppThemeBackgroundImageSource() const noexcept;
        float AppThemeBackgroundImageOpacity() const noexcept;

    private:
        IAsyncAction doCreateLiveKernelMemoryDumpAsync();
    private:
        void onSettingChanged(Windows::Foundation::IInspectable const& sender, Core::Data::EventArguments::SettingChangedEventArgs const& args);

        winrt::SystemExplorer::Core::Settings::AppearanceSettings settings_ =
           winrt::SystemExplorer::Core::Settings::UserSettings::Instance().AppearanceSettings();
    };
}
FACTORY(winrt::SystemExplorer::ViewModels, RootViewModel);