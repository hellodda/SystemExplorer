#pragma once

#include "ViewModels/RootViewModel.g.h"
#include "ViewModelBase.h"
#include "../Core/Data/EventArguments/SettingChangedEventArgs.h"
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
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
        RootViewModel();

        wil::single_threaded_property<IAsyncRelayCommand> CreateLiveKernelMemoryDumpCommand = AsyncRelayCommandFactory::Make([this](auto&) -> IAsyncAction {
            co_await doCreateLiveKernelMemoryDumpAsync();
        });

        // kkk
        Stretch AppThemeBackgroundImageFit() const noexcept;
        VerticalAlignment AppThemeBackgroundImageVerticalAlignment() const noexcept;
        HorizontalAlignment AppThemeBackgroundImageHorizontalAlignment() const noexcept;
        ImageSource AppThemeBackgroundImageSource() const noexcept;
        float AppThemeBackgroundImageOpacity() const noexcept;

    private:
        IAsyncAction doCreateLiveKernelMemoryDumpAsync();
    private:
        void onSettingChanged(IInspectable const& sender, Data::EventArguments::SettingChangedEventArgs const& args);
    };
}
FACTORY(winrt::SystemExplorer::ViewModels, RootViewModel);