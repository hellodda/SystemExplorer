#pragma once
#include <functional>
#include <winrt/Microsoft.UI.Xaml.Input.h>

namespace winrt::SystemExplorer::Helpers
{
	using namespace winrt::Windows::Foundation;
	using namespace winrt::Microsoft::UI::Xaml::Input;

	struct AsyncRelayCommand : implements<AsyncRelayCommand, ICommand>
	{
		using execute_t = std::function<IAsyncAction(IInspectable const&)>;
		using canExecute_t = std::function<bool(IInspectable const&)>;

		AsyncRelayCommand(execute_t execute, canExecute_t canExecute = nullptr);

		void Execute(IInspectable const& parameter) const;
		bool CanExecute(IInspectable const& parameter) const;

		event_token CanExecuteChanged(EventHandler<IInspectable> const& handler);
		void CanExecuteChanged(event_token const& token) noexcept;

		void RaiseCanExecuteChanged();

	private:
		execute_t execute_;
		canExecute_t canExecute_;
		event<EventHandler<IInspectable>> canExecuteChanged_;
	};
}