#pragma once

#include "ViewModels/Settings/AdvancedViewModel.g.h"
#include <winrt/SystemExplorer.Xaml.Mvvm.Input.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.Windows.Storage.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Microsoft.Windows.Storage.Pickers.h>
#include <Core/Settings/AdvancedSettings.h>
#include <fstream>
#include <App.xaml.h>

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    using namespace winrt::Windows::Storage;
    using namespace Microsoft::Windows::Storage::Pickers;
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

        wil::single_threaded_property<IAsyncRelayCommand> ExportSettingsCommand = AsyncRelayCommandFactory::Make([](auto&) -> IAsyncAction {

            auto windowId = SystemExplorer::implementation::App::Window().AppWindow().Id();
            FileSavePicker picker{ windowId };


            picker.SuggestedFileName(L"ExportedSettings");
            picker.DefaultFileExtension(L".json");
            picker.FileTypeChoices().Insert(L"JSON File", single_threaded_vector<hstring>({ L".json" }));

            auto result = co_await picker.PickSaveFileAsync();
            if (!result)
                co_return;

            Core::Settings::UserSettings::Instance().AdvancedSettings().Save();
            Core::Settings::UserSettings::Instance().AppearanceSettings().Save();

            hstring jsonContent = Core::Settings::UserSettings::Instance().ExportSettingsToJson();

            StorageFile file = co_await StorageFile::GetFileFromPathAsync(result.Path());

            co_await FileIO::WriteTextAsync(file, jsonContent);
        });
        wil::single_threaded_property<IAsyncRelayCommand> ImportSettingsCommand = AsyncRelayCommandFactory::Make([](auto&) -> IAsyncAction {

            auto windowId = SystemExplorer::implementation::App::Window().AppWindow().Id();
            FileOpenPicker picker{ windowId };

            picker.FileTypeFilter().Append(L".json");

            auto result = co_await picker.PickSingleFileAsync();
            if (!result)
                co_return;

            StorageFile file = co_await StorageFile::GetFileFromPathAsync(result.Path());

            hstring jsonContent = co_await FileIO::ReadTextAsync(file);

            if (!jsonContent.empty())
            {
                Core::Settings::UserSettings::Instance().ImportSettingsFromJson(jsonContent);
            }
        });
    };
}
FACTORY(winrt::SystemExplorer::ViewModels::Settings, AdvancedViewModel);