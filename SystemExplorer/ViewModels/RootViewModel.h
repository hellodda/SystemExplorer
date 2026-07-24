#pragma once

#include "ViewModels/RootViewModel.g.h"

#include "ViewModelBase.h"

namespace winrt
{
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Media;
    using namespace winrt::Microsoft::UI::Xaml::Media::Imaging;

    using namespace winrt::Windows::Foundation;
    using namespace winrt::SystemExplorer::Xaml::Mvvm::Input;
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

    private:
        winrt::IAsyncAction doCreateLiveKernelMemoryDumpAsync();
    };
}
FACTORY(winrt::SystemExplorer::ViewModels, RootViewModel);