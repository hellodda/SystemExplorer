#pragma once

#include "Mvvm/Input/AsyncRelayCommandFactory.g.h"

namespace winrt::SystemExplorer::Xaml::Mvvm::Input::implementation
{
    struct AsyncRelayCommandFactory : AsyncRelayCommandFactoryT<AsyncRelayCommandFactory>
    {
        AsyncRelayCommandFactory() = default;

        static IAsyncRelayCommand Make(AsyncExecuteHandler const& execute);
        static IAsyncRelayCommand Make(AsyncExecuteHandler const& execute, CanExecuteHandler const& canExecute);

        static IAsyncRelayCommand Make(AsyncExecuteHandler const& execute, AsyncRelayCommandOptions options);
        static IAsyncRelayCommand Make(AsyncExecuteHandler const& execute, CanExecuteHandler const& canExecute, AsyncRelayCommandOptions options);
    };
}

namespace winrt::SystemExplorer::Xaml::Mvvm::Input::factory_implementation
{
    struct AsyncRelayCommandFactory : AsyncRelayCommandFactoryT<AsyncRelayCommandFactory, implementation::AsyncRelayCommandFactory>
    {
    };
}
