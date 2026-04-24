#include "pch.h"
#include "ProcessesViewModel.h"
#if __has_include("ViewModels/Activities/ProcessesViewModel.g.cpp")
#include "ViewModels/Activities/ProcessesViewModel.g.cpp"
#endif

namespace winrt::SystemExplorer::ViewModels::Activities::implementation
{
    void ProcessesViewModel::updateProcessesIist(std::vector<ProcessInformation>& newProcesses)
    {
        float totalCpu{ 0 };
        uint64_t totalIo{ 0 };
        uint64_t totalPrivateBytes{ 0 };

        uint32_t selectedPid{ 0 };
        bool hasSelection{ false };

        for (const auto& proc : newProcesses)
        {
            if (proc.Name == std::wstring_view(L"Idle"))
                continue;

            totalCpu += proc.CpuUsage;
            totalIo += proc.IoRate;
            totalPrivateBytes += proc.PrivateBytes;
        }

        TotalCpuUsage(std::round(totalCpu * 100.0f) / 100.0f);
        TotalIoRate(totalIo);
        TotalPrivateBytes(totalPrivateBytes);

        if (!SelectedProcess().Name.empty())
        {
            selectedPid = SelectedProcess().Pid;
            hasSelection = true;
        }

        std::ranges::sort(newProcesses, {}, &ProcessInformation::Pid);
        std::vector<bool> processed(newProcesses.size(), false);

        uint32_t currentSize = Processes().Size();
        for (uint32_t i = currentSize; i > 0; --i)
        {
            uint32_t index = i - 1;
            ProcessInformation currentProc = Processes().GetAt(index);

            auto it = std::ranges::lower_bound(newProcesses, currentProc.Pid, {}, &ProcessInformation::Pid);

            if (it != newProcesses.end() && it->Pid == currentProc.Pid)
            {
                if (currentProc.CpuUsage != it->CpuUsage ||
                    currentProc.IoRate != it->IoRate ||
                    currentProc.PrivateBytes != it->PrivateBytes)
                {
                    Processes().SetAt(index, *it);
                }
                processed[std::distance(newProcesses.begin(), it)] = true;
            }
            else
            {
                Processes().RemoveAt(index);
            }
        }
        for (size_t i = 0; i < newProcesses.size(); ++i)
        {
            if (!processed[i])
            {
                Processes().Append(newProcesses[i]);
            }
        }
        if (hasSelection)
        {
            restoreSelection(selectedPid);
        }
    }
    void ProcessesViewModel::restoreSelection(uint32_t pid)
    {
        auto view = Processes();
        uint32_t size = view.Size();

        for (uint32_t i = 0; i < size; ++i)
        {
            if (view.GetAt(i).Pid == pid)
            {
                SelectedProcess(view.GetAt(i));
                return;
            }
        }
        SelectedProcess(ProcessInformation{});
    }
}
