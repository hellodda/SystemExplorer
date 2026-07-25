#include "pch.h"
#include "winrt_module_imports.h"
#include "SimpleSettingsGroupAutomationPeer.h"
#if __has_include("Xaml/SimpleSettingsGroupAutomationPeer.g.cpp")
#include "Xaml/SimpleSettingsGroupAutomationPeer.g.cpp"
#endif

namespace winrt::SystemExplorer::Xaml::implementation
{
    SimpleSettingsGroupAutomationPeer::SimpleSettingsGroupAutomationPeer(owner_type const& owner)
        : SimpleSettingsGroupAutomationPeerT<SimpleSettingsGroupAutomationPeer>(owner) {}

    winrt::hstring SimpleSettingsGroupAutomationPeer::GetNameCore()
    {
        if (auto owner = Owner().try_as<SystemExplorer::Xaml::SimpleSettingsGroup>())
        {
            return owner.Header();
        }
        return base_type::GetNameCore();
    }
}
