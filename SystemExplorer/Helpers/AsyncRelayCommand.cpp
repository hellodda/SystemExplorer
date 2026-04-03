#include "pch.h"
#include "AsyncRelayCommand.h"

namespace winrt::SystemExplorer::Helpers
{
	AsyncRelayCommand::AsyncRelayCommand(execute_t execute, canExecute_t canExecute)
		: execute_(execute), canExecute_(canExecute) {}

	void AsyncRelayCommand::Execute(IInspectable const& parameter) const
	{
		if (execute_)
			execute_(parameter);
	}

	bool AsyncRelayCommand::CanExecute(IInspectable const& parameter) const
	{
		return canExecute_ ? canExecute_(parameter) : true;
	}

	event_token AsyncRelayCommand::CanExecuteChanged(EventHandler<IInspectable> const& handler)
	{
		return canExecuteChanged_.add(handler);
	}

	void AsyncRelayCommand::CanExecuteChanged(event_token const& token) noexcept
	{
		canExecuteChanged_.remove(token);
	}

	void AsyncRelayCommand::RaiseCanExecuteChanged()
	{
		canExecuteChanged_(nullptr, nullptr);
	}
}