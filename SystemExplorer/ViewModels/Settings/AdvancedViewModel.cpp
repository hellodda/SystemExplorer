#include "pch.h"
#include "winrt_module_imports.h"
#include "AdvancedViewModel.h"
#if __has_include("ViewModels/Settings/AdvancedViewModel.g.cpp")
#include "ViewModels/Settings/AdvancedViewModel.g.cpp"
#endif

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    winrt::IAsyncAction AdvancedViewModel::doImportSettingsAsync()
    {
        co_return;
    }

    winrt::IAsyncAction AdvancedViewModel::doExportSettingsAsync()
    {
        co_return;
    }

    winrt::IAsyncAction AdvancedViewModel::doOpenSettingsFolderAsync()
    {
        co_return;
    }
}
