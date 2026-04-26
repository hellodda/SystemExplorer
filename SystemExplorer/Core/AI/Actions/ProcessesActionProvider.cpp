#include "pch.h"
#include "ProcessesActionProvider.h"

namespace winrt::SystemExplorer::Core::AI::Actions::implementation
{
    IAsyncAction ProcessesActionProvider::InvokeAsync(ActionInvocationContext const& context)
    {
        hstring actionId = context.ActionId();

        if (actionId == L"SystemExplorer.EndProcess")
        {
            co_await endProcessAction(context);
        }

        context.Result(ActionInvocationResult::Unavailable);
        co_return;
    }
    IAsyncAction ProcessesActionProvider::endProcessAction(ActionInvocationContext const& context)
    {
        context.Result(ActionInvocationResult::Unsupported);
        co_return;
    }
}
