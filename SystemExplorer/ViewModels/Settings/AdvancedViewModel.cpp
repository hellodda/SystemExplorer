#include "pch.h"
#include "winrt_module_imports.h"
#include "AdvancedViewModel.h"
#if __has_include("ViewModels/Settings/AdvancedViewModel.g.cpp")
#include "ViewModels/Settings/AdvancedViewModel.g.cpp"
#endif
#include <winrt/Windows.System.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.Windows.Storage.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Microsoft.Windows.Storage.Pickers.h>
#include <Core/Settings/AdvancedSettings.h>
#include <fstream>
#include <App.xaml.h>

using namespace winrt::Windows::Storage;
using namespace winrt::Microsoft::Windows::Storage::Pickers;

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    IAsyncAction AdvancedViewModel::doImportSettingsAsync()
    {
        auto windowId = SystemExplorer::CurrentApplication::Window().AppWindow().Id();
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
    }
    IAsyncAction AdvancedViewModel::doExportSettingsAsync()
    {
        auto windowId = SystemExplorer::CurrentApplication::Window().AppWindow().Id();
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
    }
    IAsyncAction AdvancedViewModel::doOpenSettingsFolderAsync()
    {
        auto localFolder = Microsoft::Windows::Storage::ApplicationData::GetDefault().LocalFolder();
        try {
            auto settingsFolder = co_await Microsoft::Windows::Storage::ApplicationData::GetDefault()
                .LocalFolder().GetParentAsync();

            auto targetFolder = co_await settingsFolder.GetFolderAsync(L"Settings");

            co_await winrt::Windows::System::Launcher::LaunchFolderAsync(targetFolder);
        }
        catch (...) {

        }
    }
}
