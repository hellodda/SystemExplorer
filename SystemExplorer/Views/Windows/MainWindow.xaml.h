#pragma once
#include "Views/Windows/MainWindow.g.h"
#include <factory.h>

namespace winrt
{
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Microsoft::UI::Windowing;
}

namespace winrt::SystemExplorer::Views::Windows::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();
    };
}
FACTORY(winrt::SystemExplorer::Views::Windows, MainWindow);

