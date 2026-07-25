#pragma once

#include "Xaml/SimpleSettingsGroupAutomationPeer.g.h"

namespace winrt::SystemExplorer::Xaml::implementation
{
    using namespace winrt;
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml::Automation;
    using namespace winrt::Microsoft::UI::Xaml::Automation::Peers;

    struct SimpleSettingsGroupAutomationPeer : SimpleSettingsGroupAutomationPeerT<SimpleSettingsGroupAutomationPeer>
    {
        using owner_type = SystemExplorer::Xaml::Controls::SimpleSettingsGroup;

        SimpleSettingsGroupAutomationPeer(owner_type const& owner);

        [[nodiscard]] hstring GetNameCore();
    };
}

namespace winrt::SystemExplorer::Xaml::factory_implementation
{
    struct SimpleSettingsGroupAutomationPeer : SimpleSettingsGroupAutomationPeerT<SimpleSettingsGroupAutomationPeer, implementation::SimpleSettingsGroupAutomationPeer>
    {
    };
}
