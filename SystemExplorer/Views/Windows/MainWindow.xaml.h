#pragma once

#include "Views/Windows/MainWindow.g.h"

namespace winrt::SystemExplorer::Views::Windows::implementation
{
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
