#pragma once
#include <winrt/Windows.AI.Actions.h>
#include <winrt/Windows.AI.Actions.Provider.h>
#include <factory.h>
/// <summary>
/// poka v planax...
/// </summary>

namespace winrt::SystemExplorer::Core::AI::Actions::implementation
{
    using namespace winrt::Windows::Foundation;

    using namespace winrt::Windows::AI::Actions;
    using namespace winrt::Windows::AI::Actions::Provider;

    struct __declspec(uuid("9f46d58d-dffa-4c00-85c2-bffb13bdcb6a")) ProcessesActionProvider : implements<ProcessesActionProvider, IActionProvider>
    {
        ProcessesActionProvider() = default;

        IAsyncAction InvokeAsync(ActionInvocationContext const& context);

    private:
        static IAsyncAction endProcessAction(ActionInvocationContext const& context);
        //static IAsyncAction createProcessAction();
        //static IAsyncAction getDetailInformationOfProcessAction();
    };
}
COM_FACTORY(winrt::SystemExplorer::Core::AI::Actions, ProcessesActionProvider); 