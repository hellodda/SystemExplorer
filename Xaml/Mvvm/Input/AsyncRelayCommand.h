#pragma once
#include "Mvvm/Input/AsyncRelayCommand.g.h"
#include <wil/cppwinrt_authoring.h>

namespace winrt::SystemExplorer::Xaml::Mvvm::Input::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml::Input;
    using namespace winrt::Microsoft::UI::Xaml::Data;

    struct AsyncRelayCommand : AsyncRelayCommandT<AsyncRelayCommand>, wil::notify_property_changed_base<AsyncRelayCommand>
    {
        AsyncRelayCommand(AsyncExecuteHandler const& execute);
        AsyncRelayCommand(AsyncExecuteHandler const& execute, AsyncRelayCommandOptions options);
        AsyncRelayCommand(AsyncExecuteHandler const& execute, CanExecuteHandler const& canExecute);
        AsyncRelayCommand(AsyncExecuteHandler const& execute, CanExecuteHandler const& canExecute, AsyncRelayCommandOptions options);

        void Execute(IInspectable const& parameter);
        bool CanExecute(IInspectable const& parameter);

        [[nodiscard]] winrt::event_token CanExecuteChanged(EventHandler<IInspectable> const& handler);
        void CanExecuteChanged(winrt::event_token const& token) noexcept;

        IAsyncAction ExecuteAsync(IInspectable const& parameter);
        void Cancel();

        bool IsCancellationSupported() { return true; }

        WIL_NOTIFYING_PROPERTY(IAsyncAction, ExecutionTask, nullptr);
        WIL_NOTIFYING_PROPERTY(bool, IsRunning, false);
        WIL_NOTIFYING_PROPERTY(bool, CanBeCanceled, false);
        WIL_NOTIFYING_PROPERTY(bool, IsCancellationRequested, false);

        void NotifyCanExecuteChanged();

    private:
        AsyncExecuteHandler execute_{ nullptr };
        CanExecuteHandler canExecute_{ nullptr };
        AsyncRelayCommandOptions options_{ AsyncRelayCommandOptions::None };

        winrt::event<EventHandler<IInspectable>> canExecuteChanged_;

        winrt::fire_and_forget AwaitAndThrowIfFailed(IAsyncAction task);
    };
}

namespace winrt::SystemExplorer::Xaml::Mvvm::Input::factory_implementation
{
    struct AsyncRelayCommand : AsyncRelayCommandT<AsyncRelayCommand, implementation::AsyncRelayCommand>
    {};
}