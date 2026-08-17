#include "pch.h"
#include "winrt_module_imports.h"
#include "RemoteAccessPage.xaml.h"
#if __has_include("Views/Pages/Settings/RemoteAccessPage.g.cpp")
#include "Views/Pages/Settings/RemoteAccessPage.g.cpp"
#endif

#include <ViewModels/Settings/RemoteAccessViewModel.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    RemoteAccessPage::RemoteAccessPage()
    {
        InitializeComponent();
    }

    winrt::IAsyncAction RemoteAccessPage::showDialogAsync()
    {
        RemoteComputerSetupDialog().XamlRoot(this->XamlRoot());
        co_await RemoteComputerSetupDialog().ShowAsync();
    }
}
