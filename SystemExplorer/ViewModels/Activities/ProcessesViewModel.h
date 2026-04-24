#pragma once

#include "ViewModels/Activities/ProcessesViewModel.g.h"
#include "../ViewModelBase.h"

#include <Core/System/ProcessInformationProvider.h>
#include <Core/Settings/UserSettings.h>
#include <Helpers/Common.h>

#include <winrt/SystemExplorer.Models.h>
#include <ranges>



namespace winrt::SystemExplorer::ViewModels::Activities::implementation
{
	using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Microsoft::UI::Xaml;

	using namespace winrt::SystemExplorer::Models;
    using namespace winrt::SystemExplorer::Core;
    using namespace winrt::SystemExplorer::Core::System;

    struct ProcessesViewModel : ProcessesViewModelT<ProcessesViewModel, ViewModels::implementation::ViewModelBase>
    {
        ProcessesViewModel()
        {
            auto speed = std::chrono::milliseconds(Core::Settings::UserSettings::Instance().GeneralSettings().RealTimeUpdateSpeedMs());


            pullTimer_.Interval(speed);
            provider_->Thread()->SetInterval(speed);

            pullTimer_.Tick([this](auto const&, auto const&)
            {
                    auto newProcesses = provider_->GetAllProcesses();
                    updateProcessesIist(newProcesses);
            });
            pullTimer_.Start();

            Core::Settings::UserSettings::Instance().GeneralSettings().SettingChanged([this](auto const&, auto const& args) {

                if (args.SettingName() == L"RealTimeUpdateSpeedMs")
                {
                    auto value = std::chrono::milliseconds(unbox_value<uint16_t>(args.NewValue()));

                    pullTimer_.Interval(value);
                    provider_->Thread()->SetInterval(value);
                }

            });
        }

        wil::single_threaded_property<IObservableVector<ProcessInformation>> Processes = single_threaded_observable_vector<ProcessInformation>();

        WIL_NOTIFYING_PROPERTY(ProcessInformation, SelectedProcess, ProcessInformation{});
        WIL_NOTIFYING_PROPERTY(float, TotalCpuUsage, 0);
        WIL_NOTIFYING_PROPERTY(uint64_t, TotalIoRate, 0);
        WIL_NOTIFYING_PROPERTY(uint64_t, TotalPrivateBytes, 0);
    private:
        void updateProcessesIist(std::vector<ProcessInformation>& newProcesses);
        void restoreSelection(uint32_t pid);
    private:
        std::shared_ptr<IProcessInformationProvider> provider_ = std::make_shared<ProcessInformationProvider>();
        DispatcherTimer pullTimer_;
    };
}
FACTORY(winrt::SystemExplorer::ViewModels::Activities, ProcessesViewModel);