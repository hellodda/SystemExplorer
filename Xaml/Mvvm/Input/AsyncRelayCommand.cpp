#include "pch.h"
#include "AsyncRelayCommand.h"

#if __has_include("Mvvm/Input/AsyncRelayCommand.g.cpp")
#include "Mvvm/Input/AsyncRelayCommand.g.cpp"
#endif

namespace winrt::SystemExplorer::Xaml::Mvvm::Input::implementation
{
    AsyncRelayCommand::AsyncRelayCommand(AsyncExecuteHandler const& execute)
        : execute_(execute) {}

    AsyncRelayCommand::AsyncRelayCommand(AsyncExecuteHandler const& execute, CanExecuteHandler const& canExecute)
        : execute_(execute), canExecute_(canExecute) {}

    AsyncRelayCommand::AsyncRelayCommand(AsyncExecuteHandler const& execute, CanExecuteHandler const& canExecute, AsyncRelayCommandOptions options)
        : execute_(execute), canExecute_(canExecute), options_(options) {}

    bool AsyncRelayCommand::CanExecute(IInspectable const& parameter)
    {
        bool canExecute = canExecute_ ? canExecute_(parameter) : true;
        bool allowConcurrent = (options_ & AsyncRelayCommandOptions::AllowConcurrentExecutions) != AsyncRelayCommandOptions::None;

        return canExecute && (allowConcurrent || !IsRunning());
    }

    void AsyncRelayCommand::Execute(IInspectable const& parameter)
    {
        auto task = ExecuteAsync(parameter);

        if ((options_ & AsyncRelayCommandOptions::FlowExceptionsToTaskScheduler) == AsyncRelayCommandOptions::None)
        {
            AwaitAndThrowIfFailed(task);
        }
    }

    IAsyncAction AsyncRelayCommand::ExecuteAsync(IInspectable const& parameter)
    {
        bool allowConcurrent = (options_ & AsyncRelayCommandOptions::AllowConcurrentExecutions) != AsyncRelayCommandOptions::None;

        if (IsRunning())
        {
            if (allowConcurrent)
                Cancel();
            else
                co_return;
        }

        IsCancellationRequested(false);
        ExecutionTask(execute_(parameter));
        IsRunning(true);
        CanBeCanceled(true);

        if (!allowConcurrent)
        {
            NotifyCanExecuteChanged();
        }

        auto guard = wil::scope_exit([&]()
        {
            IsRunning(false);
            CanBeCanceled(false);

            if (!allowConcurrent)
            {
                NotifyCanExecuteChanged();
            }
        });

        try
        {
            co_await ExecutionTask();
        }
        catch (winrt::hresult_canceled const&)
        {
            // xz xz
        }
        catch (...)
        {
            throw;
        }
    }

    void AsyncRelayCommand::Cancel()
    {
        auto task = ExecutionTask();
        if (task && task.Status() == AsyncStatus::Started)
        {
            IsCancellationRequested(true);
            task.Cancel();
            CanBeCanceled(false);
        }
    }

    winrt::fire_and_forget AsyncRelayCommand::AwaitAndThrowIfFailed(IAsyncAction task)
    {
        co_await task;
    }

    void AsyncRelayCommand::NotifyCanExecuteChanged()
    {
        canExecuteChanged_(*this, nullptr);
    }

    winrt::event_token AsyncRelayCommand::CanExecuteChanged(EventHandler<IInspectable> const& handler)
    {
        return canExecuteChanged_.add(handler);
    }

    void AsyncRelayCommand::CanExecuteChanged(winrt::event_token const& token) noexcept
    {
        canExecuteChanged_.remove(token);
    }
}