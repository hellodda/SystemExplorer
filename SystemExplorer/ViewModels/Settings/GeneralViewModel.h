#pragma once

#include "ViewModels/Settings/GeneralViewModel.g.h"
#include "../ViewModelBase.h"
#include <winrt/SystemExplorer.Xaml.Mvvm.Input.h>

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Windows::Foundation;

	using namespace winrt::SystemExplorer::Xaml::Mvvm::Input;

    struct GeneralViewModel : GeneralViewModelT<GeneralViewModel, ViewModels::implementation::ViewModelBase>
    {
        GeneralViewModel() = default;

        wil::single_threaded_property<IAsyncRelayCommand> RestartCommand = AsyncRelayCommandFactory::Make([this](auto&) -> IAsyncAction {
            co_return;
        });
        wil::single_threaded_property<IAsyncRelayCommand> CancelRestartCommand = AsyncRelayCommandFactory::Make([this](auto&) -> IAsyncAction {
            co_return;
        });

		DECLARE_PROPERTY(IInspectable, SelectedAppLanguage);

		wil::single_threaded_property<IVector<IInspectable>> AppLanguages = single_threaded_vector<IInspectable>();

        wil::single_threaded_notifying_property<int32_t> SelectedAppLanguageIndex;
    };
}
FACTORY(winrt::SystemExplorer::ViewModels::Settings, GeneralViewModel);