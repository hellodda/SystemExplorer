#include "pch.h"
#include "ProcessesViewModel.h"

#if __has_include("ViewModels/Activities/ProcessesViewModel.g.cpp")
#include "ViewModels/Activities/ProcessesViewModel.g.cpp"
#endif

#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <winrt/Windows.Storage.h>
#include <ranges>
#include <property.h>

#include <Core/System/ProcessInformationProvider.h>
#include <Core/System/ProcessManager.h>
#include <Core/System/Utils.h>
#include <Core/System/Native.h>
#include <Converters/Native/HiconToBitmapSourceConverter.h>
#include <Helpers/ProcessPropertiesHelper.h>
#include <Helpers/Win32/Native/NativeProcess.h>
#include <Helpers/Win32/ShellHelper.h>
#include <winrt/Microsoft.Windows.Storage.Pickers.h>
#include <App.xaml.h>

using namespace winrt::Microsoft::Windows::Storage::Pickers;

using namespace winrt::SystemExplorer::Helpers;
using namespace winrt::SystemExplorer::Helpers::Win32;
using namespace winrt::SystemExplorer::Helpers::Win32::Native;
using namespace winrt::SystemExplorer::Converters::Native;

namespace winrt::SystemExplorer::ViewModels::Activities::implementation
{
    namespace
    {
        void UpdateProcessItemValues(ProcessItem& target, _In_ const PSE_PROCESS_ITEM source)
        {
            if (target.CpuUsage() != source->CpuUsage) target.CpuUsage(source->CpuUsage);
            if (target.IoRate() != source->IoReadDelta.Delta) target.IoRate(source->IoReadDelta.Delta);
            if (target.PrivateBytes() != source->VmCounters.PrivateUsage) target.PrivateBytes(source->VmCounters.PrivateUsage);
        }

        ProcessItem CreateProcessItemFromNativeSource(_In_ const PSE_PROCESS_ITEM source)
        {
            ProcessItem item;
            item.Pid(reinterpret_cast<uint64_t>(source->ProcessId));
            item.ParentId(reinterpret_cast<uint64_t>(source->ParentProcessId));

            if (source->ProcessName) item.Name(source->ProcessName);
            if (source->FileName) item.Description(source->FileName);

            item.CpuUsage(source->CpuUsage);
            item.IoRate(source->IoReadDelta.Delta);
            item.PrivateBytes(source->VmCounters.PrivateUsage);
            item.IsEfficiencyModeEnabled(source->IsPowerThrottling);

            auto icon = ShellHelper::GetIconByIndex(source->SmallIconIndex);
            item.Icon(HiconToBitmapSourceConverter::Convert(std::move(icon)));

            return item;
        }
    }

    ProcessesViewModel::ProcessesViewModel()
    {
        provider_ = std::make_shared<ProcessInformationProvider>();
        manager_ = std::make_shared<ProcessManager>();

        const auto updateSpeed = std::chrono::milliseconds(
            Core::Settings::UserSettings::Instance().GeneralSettings().RealTimeUpdateSpeedMs()
        );

        pullTimer_.Interval(updateSpeed);
        provider_->Thread().SetInterval(updateSpeed);

        pullTimer_.Tick([this](const auto&, const auto&)
        {
            auto currentProcesses = provider_->GetAllProcesses();
            updateProcessesList(currentProcesses);
        });
        pullTimer_.Start();

        Core::Settings::UserSettings::Instance().GeneralSettings().SettingChanged(
            [weakThis = this->get_weak()](auto const&, auto const& args)
            {
                if (auto sharedThis = weakThis.get(); sharedThis && args.SettingName() == L"RealTimeUpdateSpeedMs")
                {
                    const auto newSpeed = std::chrono::milliseconds(unbox_value<uint16_t>(args.NewValue()));
                    sharedThis->pullTimer_.Interval(newSpeed);
                    sharedThis->provider_->Thread().SetInterval(newSpeed);
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
                    searchRegex_.emplace(
                        SearchString_.c_str(),
                        std::regex_constants::icase |
                        std::regex_constants::ECMAScript |
                        std::regex_constants::optimize
                    );
                }
                catch (const std::regex_error&) {}
            }
            applyTransformations();
        }
    }

    void ProcessesViewModel::updateProcessesList(std::vector<PSE_PROCESS_ITEM>& newProcesses)
    {
        lastRawProcesses_ = std::move(newProcesses);
        applyTransformations();
    }

    void ProcessesViewModel::applyTransformations()
    {
        std::unordered_set<uint32_t> activePids;
        activePids.reserve(lastRawProcesses_.size());

        updateMetricsAndCache(activePids);
        pruneDeadProcesses(activePids);
    }

    void ProcessesViewModel::updateMetricsAndCache(std::unordered_set<uint32_t>& outActivePids)
    {
        float totalCpu{ 0.0f };
        uint64_t totalIo{ 0 };
        uint64_t totalPrivateBytes{ 0 };

        for (const auto& process : lastRawProcesses_)
        {
            const auto pid = static_cast<uint32_t>(reinterpret_cast<ULONG_PTR>(process->ProcessId));
            outActivePids.insert(pid);

            if (process->ProcessName && std::wstring_view(process->ProcessName) != L"Idle")
            {
                totalCpu += process->CpuUsage;
                totalIo += process->IoReadDelta.Delta;
                totalPrivateBytes += process->VmCounters.PrivateUsage;
            }

            if (auto it = itemCache_.find(pid); it != itemCache_.end())
            {
                UpdateProcessItemValues(it->second, process);
            }
            else
            {
                auto processItem = CreateProcessItemFromNativeSource(process);
                itemCache_.emplace(pid, processItem);
                Processes.Append(processItem);
            }
        }

        TotalCpuUsage(std::round(totalCpu * 10.0f) / 10.0f);
        TotalIoRate(totalIo);
        TotalPrivateBytes(totalPrivateBytes);
    }

    void ProcessesViewModel::pruneDeadProcesses(std::unordered_set<uint32_t> const& activePids)
    {
        std::erase_if(itemCache_, [&](auto const& cacheEntry)
            {
                const auto& [pid, uiItem] = cacheEntry;
                if (!activePids.contains(pid))
                {
                    if (uint32_t index; Processes.IndexOf(uiItem, index))
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
        if (!SelectedProcess_) co_return;
        const auto pid = SelectedProcess_.Pid();

        try
        {
            manager_->Terminate(pid);
        }
        catch (const wil::ResultException&) {}
    }

    IAsyncAction ProcessesViewModel::doSetEfficiencyModeAsync()
    {
        if (!SelectedProcess_) co_return;
        const auto pid = SelectedProcess_.Pid();

        try
        {
            if (!NativeProcess::IsEfficiencyModeEnabled(pid))
            {
                manager_->EnableEfficiencyMode(pid);
            }
            else
            {
                manager_->DisableEfficiencyMode(pid);
            }
        }
        catch (const wil::ResultException&) {}
    }

    IAsyncAction ProcessesViewModel::doRestartProcessAsync()
    {
        if (!SelectedProcess_) co_return;
        manager_->Restart(SelectedProcess_.Pid());
        co_return;
    }

    IAsyncAction ProcessesViewModel::doOpenProcessDetailsWindowAsync()
    {
        if (SelectedProcess_)
        {
            ProcessPropertiesHelper::OpenPropertiesWindow(SelectedProcess_);
        }
        co_return;
    }

    IAsyncAction ProcessesViewModel::doOpenProcessLocationAsync()
    {
        using namespace winrt::Windows::Storage;
        if (!SelectedProcess_) co_return;

        try
        {
            const auto path = NativeProcess::GetProcessImageName(SelectedProcess_.Pid());
            auto targetFile = co_await StorageFile::GetFileFromPathAsync(path);

            if (auto parentFolder = co_await targetFile.GetParentAsync())
            {
                winrt::Windows::System::FolderLauncherOptions options;
                options.ItemsToSelect().Append(targetFile);

                co_await winrt::Windows::System::Launcher::LaunchFolderAsync(parentFolder, options);
            }
        }
        catch (const hresult_error&) {}
        catch (const wil::ResultException&) {}
    }

    IAsyncAction ProcessesViewModel::doDumpProcessMemoryAsync(MINIDUMP_TYPE dumpType)
    {
        if (!SelectedProcess_) co_return;
        const auto pid = SelectedProcess_.Pid();
        const auto processName = SelectedProcess_.Name();

        auto processItem = provider_->GetProcess(pid);
        if (!processItem) co_return;

        FileSavePicker picker{ SystemExplorer::CurrentApplication::GetWindowId() };

        picker.SuggestedFileName(processName + L"_memorydump");
        picker.DefaultFileExtension(L".dmp");

        if (auto savedFile = co_await picker.PickSaveFileAsync())
        {
            try
            {
                SeCreateDumpFileProcess(savedFile.Path().c_str(), processItem, dumpType);
            }
            catch (...) {}
        }
    }

    IAsyncAction ProcessesViewModel::showErrorDialogAsync(const hstring& message)
    {
        MessageBox(NULL, message.c_str(), L"Action failed", MB_OK | MB_ICONERROR);
        co_return;
    }
}