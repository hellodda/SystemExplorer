#pragma once

#include "Models/ProcessInformation.g.h"
#include <factory.h>

namespace winrt::SystemExplorer::Models::implementation
{
    using namespace winrt::Windows::Foundation::Collections;

    struct ProcessInformation : ProcessInformationT<ProcessInformation>, wil::notify_property_changed_base<ProcessInformation>
    {
        ProcessInformation() = default;

        WIL_NOTIFYING_PROPERTY(uint32_t, Pid, 0);
        WIL_NOTIFYING_PROPERTY(uint32_t, ParentId, 0);
        WIL_NOTIFYING_PROPERTY(uint32_t, IoRate, 0);
        WIL_NOTIFYING_PROPERTY(uint32_t, PrivateBytes, 0);
        WIL_NOTIFYING_PROPERTY(float, CpuUsage, 0);
        WIL_NOTIFYING_PROPERTY(hstring, Name, L"");
        WIL_NOTIFYING_PROPERTY(hstring, Description, L"");
    };
}
FACTORY(winrt::SystemExplorer::Models, ProcessInformation);