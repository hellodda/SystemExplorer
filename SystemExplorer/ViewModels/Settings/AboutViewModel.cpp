#include "pch.h"
#include "winrt_module_imports.h"
#include "AboutViewModel.h"
#if __has_include("ViewModels/Settings/AboutViewModel.g.cpp")
#include "ViewModels/Settings/AboutViewModel.g.cpp"
#endif

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    IAsyncAction AboutViewModel::doOpenLogsFolderAsync()
    {
        auto appDataPath = winrt::Microsoft::Windows::Storage::ApplicationData::GetDefault().LocalCacheFolder().Path();
        auto logFilePath = hstring(appDataPath + L"SystemExplorer.log");
        
        if (!logFilePath.empty())
        {
            co_await Launcher::LaunchUriAsync(Uri{ hstring{ L"file:///" } + logFilePath });
        }
    }
}

