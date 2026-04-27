#include "pch.h"
#include "WmiActionProvider.h"

#include <Core/System/Management.h>

using namespace winrt::SystemExplorer::Core::System::Management;

namespace winrt::SystemExplorer::Core::AI::Actions::implementation
{
	IAsyncAction WmiActionProvider::InvokeAsync(ActionInvocationContext const& context)
	{
		hstring actionId = context.ActionId();

		if (actionId == L"SystemExplorer.QueryWQL")
		{
			co_await queryWqlAction(context);
		}

		context.Result(ActionInvocationResult::Unavailable);
		co_return;
	}
	IAsyncAction WmiActionProvider::queryWqlAction(ActionInvocationContext const& context)
	{
		context.Result(ActionInvocationResult::Unsupported);
		co_return;
	}
}