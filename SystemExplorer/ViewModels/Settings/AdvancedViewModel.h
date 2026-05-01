#pragma once

#include "ViewModels/Settings/AdvancedViewModel.g.h"
#include <winrt/SystemExplorer.Xaml.Mvvm.Input.h>
#include <factory.h>
#include <property.h>

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::System;

    using namespace winrt::SystemExplorer::Xaml::Mvvm::Input;

    struct AdvancedViewModel : AdvancedViewModelT<AdvancedViewModel>
    {
        AdvancedViewModel() = default;

        wil::single_threaded_property<IAsyncRelayCommand> OpenSettingsFileCommand = AsyncRelayCommandFactory::Make([this](auto&)-> IAsyncAction {
            co_await doOpenSettingsFolderAsync();
        });

        wil::single_threaded_property<IAsyncRelayCommand> ExportSettingsCommand = AsyncRelayCommandFactory::Make([this](auto&) -> IAsyncAction {
            co_await doExportSettingsAsync();
        });
        wil::single_threaded_property<IAsyncRelayCommand> ImportSettingsCommand = AsyncRelayCommandFactory::Make([this](auto&) -> IAsyncAction {
            co_await doImportSettingsAsync();
        });
    private:
        IAsyncAction doImportSettingsAsync();
        IAsyncAction doExportSettingsAsync();
        IAsyncAction doOpenSettingsFolderAsync();
    };
}
FACTORY(winrt::SystemExplorer::ViewModels::Settings, AdvancedViewModel);