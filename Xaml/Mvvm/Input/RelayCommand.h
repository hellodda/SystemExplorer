#pragma once

#include "Mvvm/Input/RelayCommand.g.h"
#include <wil/cppwinrt_authoring.h>

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml::Input;
}

namespace winrt::SystemExplorer::Xaml::Mvvm::Input::implementation
{
    struct RelayCommand : RelayCommandT<RelayCommand>
    {
        RelayCommand(ExecuteHandler const& execute)
            : execute_(execute) {}
        RelayCommand(ExecuteHandler const& execute, CanExecuteHandler const& canExecute)
            : execute_(execute), canExecute_(canExecute) {};

        void Execute(IInspectable const& parameter) const;
        [[nodiscard]] bool CanExecute(IInspectable const& parameter) const;

        void NotifyCanExecuteChanged();
        wil::untyped_event<IInspectable> CanExecuteChanged;
    private:
        ExecuteHandler execute_{ nullptr };
		CanExecuteHandler canExecute_{ nullptr };
    };
}

namespace winrt::SystemExplorer::Xaml::Mvvm::Input::factory_implementation
{
    struct RelayCommand : RelayCommandT<RelayCommand, implementation::RelayCommand>
    {
    };
}
