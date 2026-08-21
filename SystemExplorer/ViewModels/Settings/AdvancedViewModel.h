#pragma once

#include "ViewModels/Settings/AdvancedViewModel.g.h"

#include "../ViewModelBase.h"

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::SystemExplorer::Xaml;
}

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    struct AdvancedViewModel : AdvancedViewModelT<AdvancedViewModel, ViewModelBase>
    {
        AdvancedViewModel() = default;

        wil::single_threaded_property<winrt::IAsyncRelayCommand> OpenSettingsFileCommand = winrt::AsyncRelayCommandFactory::Make([this](auto&) -> winrt::IAsyncAction {
            co_await doOpenSettingsFolderAsync();
        });

        wil::single_threaded_property<winrt::IAsyncRelayCommand> ExportSettingsCommand = winrt::AsyncRelayCommandFactory::Make([this](auto&) -> winrt::IAsyncAction {
            co_await doExportSettingsAsync();
        });

        wil::single_threaded_property<winrt::IAsyncRelayCommand> ImportSettingsCommand = winrt::AsyncRelayCommandFactory::Make([this](auto&) -> winrt::IAsyncAction {
            co_await doImportSettingsAsync();
        });

        DECLARE_PROPERTY(bool, UseDriverAsDataSource, false);

    private:
        void showWarningToastNotification();
    private:
        [[nodiscard]] winrt::IAsyncAction doImportSettingsAsync();
        [[nodiscard]] winrt::IAsyncAction doExportSettingsAsync();
        [[nodiscard]] winrt::IAsyncAction doOpenSettingsFolderAsync();
    };
}
FACTORY(winrt::SystemExplorer::ViewModels::Settings, AdvancedViewModel);