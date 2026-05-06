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

    void ProcessesViewModel::SelectedProcess(ProcessInformation const& value) noexcept
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
        if (lastRawProcesses_.empty()) return;

        namespace views = std::ranges::views;

        float totalCpu = 0.0f;
        uint64_t totalIo = 0, totalPrivateBytes = 0;

        auto isNotIdle = [](const auto& p) { return std::wstring_view(p.Name) != L"Idle"; };

        for (const auto& proc : lastRawProcesses_ | views::filter(isNotIdle))
        {
            totalCpu += proc.CpuUsage;
            totalIo += proc.IoRate;
            totalPrivateBytes += proc.PrivateBytes;
        }

        TotalCpuUsage(std::round(totalCpu * 10.0f) / 10.0f);
        TotalIoRate(totalIo);
        TotalPrivateBytes(totalPrivateBytes);

        std::wstring rawSearchLower;
        if (!SearchString_.empty() && !searchRegex_.has_value())
        {
            rawSearchLower = SearchString_.c_str();
            std::ranges::transform(rawSearchLower, rawSearchLower.begin(), ::towlower);
        }

        auto passesSearchFilter = [&](const ProcessNativeInformation& proc)
        {
            if (SearchString_.empty())
                return true;

            if (searchRegex_.has_value())
            {
                return std::regex_search(proc.Name, searchRegex_.value()) ||
                    std::regex_search(std::to_wstring(proc.Pid), searchRegex_.value()) ||
                    std::regex_search(proc.Description, searchRegex_.value());
            }

            auto icase_cmp = [](wchar_t a, wchar_t b) { return std::towlower(a) == std::towlower(b); };

            if (std::search(proc.Name.begin(), proc.Name.end(), rawSearchLower.begin(), rawSearchLower.end(), icase_cmp) != proc.Name.end())
                return true;

            auto pidStr = std::to_wstring(proc.Pid);
            if (std::search(pidStr.begin(), pidStr.end(), rawSearchLower.begin(), rawSearchLower.end(), icase_cmp) != pidStr.end())
                return true;

            return false;
        };

        std::vector<ProcessNativeInformation> viewList;
        viewList.reserve(lastRawProcesses_.size());

        for (const auto& proc : lastRawProcesses_ | views::filter(passesSearchFilter))
        {
            viewList.push_back(proc);
        }

        sortManager_.Sort(viewList, [](const auto& a, const auto& b, SortColumn col) -> std::partial_ordering
        {
            std::partial_ordering cmp = std::partial_ordering::equivalent;

            switch (col)
            {
            case SortColumn::Name:         cmp = std::wstring_view(a.Name) <=> std::wstring_view(b.Name); break;
            case SortColumn::Pid:          cmp = a.Pid <=> b.Pid; break;
            case SortColumn::Cpu:          cmp = a.CpuUsage <=> b.CpuUsage; break;
            case SortColumn::IoRate:       cmp = a.IoRate <=> b.IoRate; break;
            case SortColumn::PrivateBytes: cmp = a.PrivateBytes <=> b.PrivateBytes; break;
            case SortColumn::None:         break;
            }

            if (cmp == std::partial_ordering::equivalent)
            {
                return a.Pid <=> b.Pid;
            }
            return cmp;
        });

        auto incomingPids = lastRawProcesses_
            | views::transform(&ProcessNativeInformation::Pid)
            | std::ranges::to<std::unordered_set<uint32_t>>();

        for (const auto& proc : lastRawProcesses_)
        {
            if (auto it = uiCache_.find(proc.Pid); it != uiCache_.end())
            {
                auto& uiObj = it->second;

                if (uiObj.CpuUsage() != proc.CpuUsage) uiObj.CpuUsage(proc.CpuUsage);
                if (uiObj.IoRate() != proc.IoRate) uiObj.IoRate(proc.IoRate);
                if (uiObj.PrivateBytes() != proc.PrivateBytes) uiObj.PrivateBytes(proc.PrivateBytes);
                if (uiObj.IsEfficiencyModeEnabled() != proc.IsEfficiencyModeEnabled) uiObj.IsEfficiencyModeEnabled(proc.IsEfficiencyModeEnabled);
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
                newUiObj.IsEfficiencyModeEnabled(proc.IsEfficiencyModeEnabled);
                newUiObj.Icon(converter_.Convert(proc.Icon));

                uiCache_.emplace(proc.Pid, newUiObj);
            }
        }

        std::erase_if(uiCache_, [&](const auto& pair) {
            return !incomingPids.contains(pair.first);
        });

        std::vector<ProcessInformation> newOrder;
        newOrder.reserve(viewList.size());
        for (const auto& nativeProc : viewList)
        {
            newOrder.push_back(uiCache_[nativeProc.Pid]);
        }

        auto currentObservable = Processes();
        bool needsReorder = (currentObservable.Size() != newOrder.size());

        if (!needsReorder)
        {
            for (uint32_t i = 0; i < newOrder.size(); ++i)
            {
                if (currentObservable.GetAt(i) != newOrder[i])
                {
                    needsReorder = true;
                    break;
                }
            }
        }

        if (needsReorder)
        {
            auto currentSelected = SelectedProcess();
            currentObservable.ReplaceAll(newOrder);
            if (currentSelected) {
                SelectedProcess(currentSelected);
            }
        }
    }

    void ProcessesViewModel::SortByPid() { sortManager_.Toggle(SortColumn::Pid, Sorting::SortDirection::Ascending); applyTransformations(); }
    void ProcessesViewModel::SortByName() { sortManager_.Toggle(SortColumn::Name, Sorting::SortDirection::Ascending); applyTransformations(); }
    void ProcessesViewModel::SortByCpu() { sortManager_.Toggle(SortColumn::Cpu); applyTransformations(); }
    void ProcessesViewModel::SortByIoRate() { sortManager_.Toggle(SortColumn::IoRate); applyTransformations(); }
    void ProcessesViewModel::SortByPrivateBytes() { sortManager_.Toggle(SortColumn::PrivateBytes); applyTransformations(); }


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