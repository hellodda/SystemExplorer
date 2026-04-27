#pragma once
#include <winrt/Windows.AI.Actions.h>
#include <winrt/Windows.AI.Actions.Provider.h>

namespace winrt::SystemExplorer::Core::AI::Actions::implementation
{
    using namespace winrt::Windows::Foundation;

    using namespace winrt::Windows::AI::Actions;
    using namespace winrt::Windows::AI::Actions::Provider;

    struct __declspec(uuid("00000000-0000-0000-0000-000000000000")) WmiActionProvider : implements<WmiActionProvider, IActionProvider>
    {
        WmiActionProvider() = default;

        IAsyncAction InvokeAsync(ActionInvocationContext const& context);

    private:
        static IAsyncAction queryWqlAction(ActionInvocationContext const& context);
        //static IAsyncAction createProcessAction();
        //static IAsyncAction getDetailInformationOfProcessAction();
    };
}
COM_FACTORY(winrt::SystemExplorer::Core::AI::Actions, WmiActionProvider);