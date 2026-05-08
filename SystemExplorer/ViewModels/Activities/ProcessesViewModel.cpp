#include "pch.h"
#include "ProcessesViewModel.h"
#if __has_include("ViewModels/Activities/ProcessesViewModel.g.cpp")
#include "ViewModels/Activities/ProcessesViewModel.g.cpp"
#endif
#include <cwctype>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <ranges>
#include <property.h>
#include <Core/System/ProcessInformationProvider.h>
#include <Core/System/ProcessManager.h>
#include <Core/System/Utils.h>
#include <Helpers/ProcessPropertiesHelper.h>

using namespace winrt::SystemExplorer::Helpers;

namespace winrt::SystemExplorer::ViewModels::Activities::implementation
{
    ProcessesViewModel::ProcessesViewModel()
    {
        provider_ = std::make_shared<ProcessInformationProvider>();
        manager_ = std::make_shared<ProcessManager>();

        auto speed = std::chrono::milliseconds(Core::Settings::UserSettings::Instance().GeneralSettings().RealTimeUpdateSpeedMs());

        pullTimer_.Interval(speed);
        provider_->Thread().SetInterval(speed);

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
                    strong_this->provider_->Thread().SetInterval(value);
                }
            }
        });
    }

    void ProcessesViewModel::SelectedProcess(ProcessItem const& value) noexcept
    {
        if (value != SelectedProcess_)
        {
            SelectedProcess_ = value;
            TerminateProcessCommand.NotifyCanExecuteChanged();
            EfficiencyModeCommand.NotifyCanExecuteChanged();

            RAISE_PROPERTY_CHANGED;
        }
    }

    void ProcessesViewModel::SearchString(hstring const& value) noexcept
    {
        if (SearchString_ != value)
        {
            SearchString_ = value;
            searchRegex_.reset();

            if (!SearchString_.empty())
            {
                try
                {
                    searchRegex_.emplace(SearchString_.c_str(),
                        std::regex_constants::icase | std::regex_constants::ECMAScript | std::regex_constants::optimize);
                }
                catch (const std::regex_error&) {}
            }

            RAISE_PROPERTY_CHANGED;
            applyTransformations();
        }
    }

    void ProcessesViewModel::updateProcessesList(std::vector<ProcessNativeInformation>& newProcesses)
    {
        lastRawProcesses_ = std::move(newProcesses);
        applyTransformations();
    }

    void ProcessesViewModel::applyTransformations()
    {
        namespace view = std::ranges::views;

        auto activeProcesses = lastRawProcesses_ | view::filter([](const auto& p) {
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

        auto incomingPids = lastRawProcesses_
            | view::transform([](const auto& p) { return p.Pid; })
            | std::ranges::to<std::unordered_set<uint32_t>>();

        for (const auto& proc : lastRawProcesses_)
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
                ProcessItem newUiObj;
                newUiObj.Pid(proc.Pid);
                newUiObj.ParentId(proc.ParentId);
                newUiObj.Name(proc.Name);
                newUiObj.Description(proc.Description);
                newUiObj.CpuUsage(proc.CpuUsage);
                newUiObj.IoRate(proc.IoRate);
                newUiObj.PrivateBytes(proc.PrivateBytes);
                newUiObj.IsEfficiencyModeEnabled(proc.IsEfficiencyModeEnabled);
                newUiObj.Icon(converter_.Convert(proc.Icon));

                uiCache_.emplace(proc.Pid, newUiObj);
                Processes.Append(newUiObj);
            }
        }

        std::erase_if(uiCache_, [&](const auto& pair)
        {
            const auto& [pid, uiObj] = pair;
            if (!incomingPids.contains(pid))
            {
                uint32_t index;
                if (Processes.IndexOf(uiObj, index))
                {
                    Processes.RemoveAt(index);
                }
                return true;
            }
            return false;
        });
    }

    // commands impl
    IAsyncAction ProcessesViewModel::doTerminateProcessAsync()
    {
        manager_->Terminate(SelectedProcess_.Pid());
        co_return;
    }

    IAsyncAction ProcessesViewModel::doSetEfficiencyModeAsync()
    {
        auto pid = SelectedProcess_.Pid();
        if (!Win32Helper::ProcessHelper::IsProcessEfficiencyModeEnabled(pid))
        {
            manager_->EnableEfficiencyMode(pid);
        }
        else
        {
            manager_->DisableEfficiencyMode(pid);
        }
        co_return;
    }

    IAsyncAction ProcessesViewModel::doRestartProcessAsync()
    {
        manager_->Restart(SelectedProcess_.Pid());
        co_return;
    }
    IAsyncAction ProcessesViewModel::doOpenProcessDetailsWindowAsync()
    {
        if (!SelectedProcess_)
            co_return;

        ProcessPropertiesHelper::OpenPropertiesWindow(SelectedProcess_);
        co_return;
    }
}