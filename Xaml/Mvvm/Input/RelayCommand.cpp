#include "pch.h"
#include "RelayCommand.h"
#if __has_include("RelayCommand.g.cpp")
#include "RelayCommand.g.cpp"
#endif

namespace winrt::SystemExplorer::Xaml::implementation
{
	void RelayCommand::Execute(IInspectable const& parameter) const
	{
		execute_(parameter);
	}

	bool RelayCommand::CanExecute(IInspectable const& parameter) const
	{
		return canExecute_ ? canExecute_(parameter) : true;
	}

	void RelayCommand::NotifyCanExecuteChanged() 
	{
		canExecuteChanged_(*this, nullptr);
	}
	event_token RelayCommand::CanExecuteChanged(EventHandler<IInspectable> const& handler)
	{
		return canExecuteChanged_.add(handler);
	}
	void RelayCommand::CanExecuteChanged(event_token const& token) noexcept
	{
		canExecuteChanged_.remove(token);
	}
}
