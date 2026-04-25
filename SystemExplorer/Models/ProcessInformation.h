#pragma once

#include "Models/ProcessInformation.g.h"

namespace winrt::SystemExplorer::Models::implementation
{
    using namespace winrt::Windows::Foundation::Collections;

    template<typename T>
    using rw_property = wil::single_threaded_rw_property<T>;

    struct ProcessInformation : ProcessInformationT<ProcessInformation>
    {
        ProcessInformation() = default;
        ProcessInformation(PSYSTEM_PROCESS_INFORMATION source) {}

        rw_property<uint32_t> Pid;
        rw_property<uint32_t> ParentId;
        rw_property<uint32_t> IoRate;
        rw_property<uint32_t> PrivateBytes;
        rw_property<float> CpuUsage;
        rw_property<hstring> Name;
        rw_property<hstring> Description;
    };
}
FACTORY(winrt::SystemExplorer::Models, ProcessInformation);