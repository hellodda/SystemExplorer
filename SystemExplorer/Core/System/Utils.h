#pragma once
#include "System.h"

namespace winrt::SystemExplorer::Core::System::Utils
{
    bool RestartProcess(
        _In_ HANDLE Process
    );
}

