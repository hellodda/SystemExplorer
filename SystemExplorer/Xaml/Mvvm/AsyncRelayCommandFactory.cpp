#include "pch.h"
#include "winrt_module_imports.h"
#include "AsyncRelayCommandFactory.h"
#if __has_include("Xaml/AsyncRelayCommandFactory.g.cpp")
#include "Xaml/AsyncRelayCommandFactory.g.cpp"
#endif
#include "AsyncRelayCommand.h"

namespace winrt::SystemExplorer::Xaml::implementation
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
