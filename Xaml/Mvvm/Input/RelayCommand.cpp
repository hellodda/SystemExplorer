#include "pch.h"
#include "winrt_module_imports.h"
#include "RelayCommand.h"
#if __has_include("Mvvm/Input/RelayCommand.g.cpp")
#include "Mvvm/Input/RelayCommand.g.cpp"
#endif

namespace winrt::SystemExplorer::Xaml::Mvvm::Input::implementation
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
		CanExecuteChanged.invoke(*this, nullptr);
	}
}
