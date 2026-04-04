#pragma once

#include "RelayCommand.g.h"

namespace winrt::SystemExplorer::Xaml::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml::Input;

    struct RelayCommand : RelayCommandT<RelayCommand>
    {
        RelayCommand(ExecuteHandler const& execute)
            : execute_(execute) {}
        RelayCommand(ExecuteHandler const& execute, CanExecuteHandler const& canExecute)
            : execute_(execute), canExecute_(canExecute) {};

        void Execute(IInspectable const& parameter) const;
        bool CanExecute(IInspectable const& parameter) const;

        void NotifyCanExecuteChanged();

        [[nodiscard]] event_token CanExecuteChanged(EventHandler<IInspectable> const& handler);
        void CanExecuteChanged(event_token const& token) noexcept;

    private:
        ExecuteHandler execute_{ nullptr };
		CanExecuteHandler canExecute_{ nullptr };

        event<EventHandler<IInspectable>> canExecuteChanged_;
    };
}

namespace winrt::SystemExplorer::Xaml::factory_implementation
{
    struct RelayCommand : RelayCommandT<RelayCommand, implementation::RelayCommand>
    {
    };
}
