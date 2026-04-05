#include "pch.h"
#include "AsyncRelayCommandFactory.h"
#if __has_include("Mvvm/Input/AsyncRelayCommandFactory.g.cpp")
#include "Mvvm/Input/AsyncRelayCommandFactory.g.cpp"
#endif
#include "AsyncRelayCommand.h"

namespace winrt::SystemExplorer::Xaml::Mvvm::Input::implementation
{
    IAsyncRelayCommand AsyncRelayCommandFactory::Make(AsyncExecuteHandler const& execute)
    {
        return make<implementation::AsyncRelayCommand>(execute);
    }
    IAsyncRelayCommand AsyncRelayCommandFactory::Make(AsyncExecuteHandler const& execute, CanExecuteHandler const& canExecute)
    {
        return make<implementation::AsyncRelayCommand>(execute, canExecute);
    }
    IAsyncRelayCommand AsyncRelayCommandFactory::Make(AsyncExecuteHandler const& execute, AsyncRelayCommandOptions options)
    {
        return make<implementation::AsyncRelayCommand>(execute, options);
    }
    IAsyncRelayCommand AsyncRelayCommandFactory::Make(AsyncExecuteHandler const& execute, CanExecuteHandler const& canExecute, AsyncRelayCommandOptions options)
    {
        return make<implementation::AsyncRelayCommand>(execute, canExecute, options);
    }
}
