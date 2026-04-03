#include "pch.h"
#include "SimpleSettingsGroupAutomationPeer.h"
#if __has_include("Controls.SimpleSettingsGroupAutomationPeer.g.cpp")
#include "Controls.SimpleSettingsGroupAutomationPeer.g.cpp"
#endif

namespace winrt::SystemExplorer::Xaml::Controls::implementation
{
    SimpleSettingsGroupAutomationPeer::SimpleSettingsGroupAutomationPeer(owner_type const& owner)
        : SimpleSettingsGroupAutomationPeerT<SimpleSettingsGroupAutomationPeer>(owner) {}

    winrt::hstring SimpleSettingsGroupAutomationPeer::GetNameCore()
    {
        if (auto owner = Owner().try_as<SystemExplorer::Xaml::Controls::SimpleSettingsGroup>())
        {
            return owner.Header();
        }
        return base_type::GetNameCore();
    }
}
