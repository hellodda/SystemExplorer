#include "pch.h"
#include "ProcessesViewModel.h"
#if __has_include("ViewModels/Activities/ProcessesViewModel.g.cpp")
#include "ViewModels/Activities/ProcessesViewModel.g.cpp"
#endif
#include <ranges>
#include <property.h>

namespace winrt::SystemExplorer::ViewModels::Activities::implementation
{
    ProcessesViewModel::ProcessesViewModel()
    {
        auto speed = std::chrono::milliseconds(Core::Settings::UserSettings::Instance().GeneralSettings().RealTimeUpdateSpeedMs());

        pullTimer_.Interval(speed);
        provider_->Thread()->SetInterval(speed);

        pullTimer_.Tick([this](auto const&, auto const&)
        {
            auto newProcesses = provider_->GetAllProcesses();
            updateProcessesList(newProcesses);
        });
        pullTimer_.Start();

        Core::Settings::UserSettings::Instance().GeneralSettings().SettingChanged([weak_this = this->get_weak()](auto const&, auto const& args)
        {
            if (auto strong_this = weak_this.get())
            {
                if (args.SettingName() == L"RealTimeUpdateSpeedMs")
                {
                    auto value = std::chrono::milliseconds(unbox_value<uint16_t>(args.NewValue()));
                    strong_this->pullTimer_.Interval(value);
                    strong_this->provider_->Thread()->SetInterval(value);
                }
            }
        });

        //TerminateProcessCommand = AsyncRelayCommandFactory::Make(&ProcessesViewModel::terminateProcessAsync);
    }

    void ProcessesViewModel::SelectedProcess(ProcessInformation const& value) noexcept
    {
        if (value != SelectedProcess_)
        {
            SelectedProcess_ = value;
            TerminateProcessCommand.NotifyCanExecuteChanged();

            RAISE_PROPERTY_CHANGED;
        }
    }

    void ProcessesViewModel::updateProcessesList(std::vector<PROCESS_INFORMATION>& newProcesses)
    {
        namespace view = std::ranges::views;

        auto activeProcesses = newProcesses | view::filter([](const auto& p) {
            return std::wstring_view(p.Name) != L"Idle";
        });

        float totalCpu = 0;
        uint64_t totalIo = 0, totalPrivateBytes = 0;

        for (const auto& proc : activeProcesses) {
            totalCpu += proc.CpuUsage;
            totalIo += proc.IoRate;
            totalPrivateBytes += proc.PrivateBytes;
        }

        TotalCpuUsage(std::round(totalCpu * 10.0f) / 10.0f);
        TotalIoRate(totalIo);
        TotalPrivateBytes(totalPrivateBytes);

        auto incomingPids = newProcesses
            | view::transform([](const auto& p) { return p.Pid; })
            | std::ranges::to<std::unordered_set<uint32_t>>(); 

        for (const auto& proc : newProcesses)
        {
            if (auto it = uiCache_.find(proc.Pid); it != uiCache_.end())
            {
                auto& uiObj = it->second;
                if (uiObj.CpuUsage() != proc.CpuUsage) uiObj.CpuUsage(proc.CpuUsage);
                if (uiObj.IoRate() != proc.IoRate) uiObj.IoRate(proc.IoRate);
                if (uiObj.PrivateBytes() != proc.PrivateBytes) uiObj.PrivateBytes(proc.PrivateBytes);
            }
            else
            {
                ProcessInformation newUiObj;
                newUiObj.Pid(proc.Pid);
                newUiObj.ParentId(proc.ParentId);
                newUiObj.Name(proc.Name);
                newUiObj.Description(proc.Description);
                newUiObj.CpuUsage(proc.CpuUsage);
                newUiObj.IoRate(proc.IoRate);
                newUiObj.PrivateBytes(proc.PrivateBytes);

                uiCache_.emplace(proc.Pid, newUiObj);
                Processes().Append(newUiObj);
            }
        }

        std::erase_if(uiCache_, [&](const auto& pair) {
            const auto& [pid, uiObj] = pair;
            if (!incomingPids.contains(pid)) {
                uint32_t index;
                if (Processes().IndexOf(uiObj, index)) {
                    Processes().RemoveAt(index);
                }
                return true;
            }
            return false;
        });
    }

    // commands impl

    IAsyncAction ProcessesViewModel::doTerminateProcessAsync()
    {
        if (!SelectedProcess_)
            co_return;

        wil::unique_process_handle handle{ OpenProcess(PROCESS_TERMINATE, FALSE, SelectedProcess_.Pid()) };

        if (!TerminateProcess(handle.get(), EXIT_SUCCESS))
        {
            THROW_LAST_ERROR_MSG("Failed \"failed terminate processs :(\" ");
        }
        co_return;
    }
}
