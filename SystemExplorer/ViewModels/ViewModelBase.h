#pragma once

namespace winrt::SystemExplorer::ViewModels
{
	using namespace Microsoft::UI::Dispatching;

	struct ViewModelBase
	{
	protected:
		DispatcherQueue dispatcherQueue_ = DispatcherQueue::GetForCurrentThread();
	};

	template<typename TViewModel>
	struct BindableViewModelBase : ViewModelBase, wil::notify_property_changed_base<TViewModel> {};
}