#pragma once

#include "Core/Data/Items/ProcessItem.g.h"

#ifdef __INTELLISENSE__
#include <wil/cppwinrt_authoring.h>
#endif
#include <wil/cppwinrt_authoring.h>


namespace winrt::SystemExplorer::Core::Data::Items::implementation
{
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Microsoft::UI::Xaml::Media;

    struct ProcessItem : ProcessItemT<ProcessItem>, wil::notify_property_changed_base<ProcessItem>
    {
        ProcessItem() = default;

        wil::single_threaded_rw_property<uint64_t> Handle;

        WIL_NOTIFYING_PROPERTY(uint32_t, Pid, 0);
        WIL_NOTIFYING_PROPERTY(uint32_t, ParentId, 0);
        WIL_NOTIFYING_PROPERTY(uint32_t, IoRate, 0);
        WIL_NOTIFYING_PROPERTY(uint32_t, PrivateBytes, 0);
        WIL_NOTIFYING_PROPERTY(float, CpuUsage, 0);
        WIL_NOTIFYING_PROPERTY(bool, IsEfficiencyModeEnabled, false);
        WIL_NOTIFYING_PROPERTY(bool, IsTerminated, false);
        WIL_NOTIFYING_PROPERTY(hstring, Name, L"");
        WIL_NOTIFYING_PROPERTY(hstring, Description, L"");
        WIL_NOTIFYING_PROPERTY(ImageSource, Icon, nullptr);
    };
}
FACTORY(winrt::SystemExplorer::Core::Data::Items, ProcessItem);

