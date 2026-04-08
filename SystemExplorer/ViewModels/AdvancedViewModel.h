#pragma once

#include "ViewModels/AdvancedViewModel.g.h"
#include <winrt/SystemExplorer.Xaml.Mvvm.Input.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Microsoft.Windows.Storage.h>
#include <Core/Settings/AdvancedSettings.h>

namespace winrt::SystemExplorer::ViewModels::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::System;
    using namespace winrt::SystemExplorer::Xaml::Mvvm::Input;

    struct AdvancedViewModel : AdvancedViewModelT<AdvancedViewModel>
    {
        AdvancedViewModel() = default;

        wil::single_threaded_property<IAsyncRelayCommand> OpenSettingsFileCommand = AsyncRelayCommandFactory::Make([](auto&)-> IAsyncAction {
            auto localFolder = Microsoft::Windows::Storage::ApplicationData::GetDefault().LocalFolder();
            try {
                auto settingsFolder = co_await Microsoft::Windows::Storage::ApplicationData::GetDefault()
                    .LocalFolder().GetParentAsync();

                auto targetFolder = co_await settingsFolder.GetFolderAsync(L"Settings");

                co_await winrt::Windows::System::Launcher::LaunchFolderAsync(targetFolder);
            }
            catch (...) {
                
            }
        });

        Core::Settings::AdvancedSettings Settings()
        {
            return Core::Settings::UserSettings::Instance().AdvancedSettings();
        }

        //wil::single_threaded_property<Core::Settings::AdvancedSettings> Settings;
    };
}

namespace winrt::SystemExplorer::ViewModels::factory_implementation
{
    struct AdvancedViewModel : AdvancedViewModelT<AdvancedViewModel, implementation::AdvancedViewModel>
    {
    };
}
