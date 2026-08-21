#pragma once

#include "ViewModels/Settings/GeneralViewModel.g.h"
#include "../ViewModelBase.h"

#include <winrt/Microsoft.Windows.AppLifecycle.h>
#include <Core/Data/Items/AppLanguageItem.h>

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::Windows::AppLifecycle;

	using namespace winrt::SystemExplorer::Xaml;
	using namespace winrt::SystemExplorer::Core::Data::Items;

    struct GeneralViewModel : GeneralViewModelT<GeneralViewModel, ViewModels::implementation::ViewModelBase>
    {
        GeneralViewModel();

        wil::single_threaded_property<IAsyncRelayCommand> RestartCommand = AsyncRelayCommandFactory::Make([this](auto&) -> IAsyncAction {
            AppInstance::Restart(L"");
            co_return;
        });
        wil::single_threaded_property<IAsyncRelayCommand> CancelRestartCommand = AsyncRelayCommandFactory::Make([this](auto&) -> IAsyncAction {
            ShowRestartControl = false;
            co_return;
        });
        
        DECLARE_ONLY_SETTER(int32_t, SelectedAppLanguageIndex);
        DECLARE_ONLY_SETTER(int32_t, SelectedRealTimeUpdateSpeedIndex);
        DECLARE_PROPERTY(bool, AppWindowAlwaysOnTop, false);
        DECLARE_PROPERTY(bool, AppWindowMinimizeOnUse, false);
        DECLARE_PROPERTY(bool, AppWindowHideWhenMinimized, false);

        wil::single_threaded_rw_property<int32_t> SelectedStartPageIndex;

		wil::single_threaded_property<IVector<IInspectable>> AppLanguages = single_threaded_vector<IInspectable>();
		wil::single_threaded_property<IVector<IInspectable>> StartPages = single_threaded_vector<IInspectable>();
        wil::single_threaded_notifying_property<bool> ShowRestartControl;
    private:
		std::vector<AppLanguageItem> appLanguages_;
    };
}
FACTORY(winrt::SystemExplorer::ViewModels::Settings, GeneralViewModel);