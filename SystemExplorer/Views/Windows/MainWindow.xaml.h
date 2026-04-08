#pragma once

#include "Views/Windows/MainWindow.g.h"
#include <Core/Settings/UserSettings.h>
#include <Core/Services/AppResourcesService.h>


namespace winrt::SystemExplorer::Views::Windows::implementation
{
    using namespace winrt::Windows::Foundation::Collections;

    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow()
        {
            ExtendsContentIntoTitleBar(true);
        }
    };
}

namespace winrt::SystemExplorer::Views::Windows::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
