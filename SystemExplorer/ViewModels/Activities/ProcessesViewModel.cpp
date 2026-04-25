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

        std::unordered_map<uint32_t, ProcessInformation> incomingMap;
        incomingMap.reserve(newProcesses.size());

        for (const auto& proc : newProcesses)
        {
            if (proc.Name() != L"Idle")
            {
                totalCpu += proc.CpuUsage();
                totalIo += proc.IoRate();
                totalPrivateBytes += proc.PrivateBytes();
            }
            incomingMap.emplace(proc.Pid(), proc);
        }

        TotalCpuUsage(std::round(totalCpu * 100.0f) / 100.0f);
        TotalIoRate(totalIo);
        TotalPrivateBytes(totalPrivateBytes);

        for (auto it = m_uiMap.begin(); it != m_uiMap.end(); )
        {
            uint32_t pid = it->first;
            auto& existingProc = it->second;

            auto incomingIt = incomingMap.find(pid);
            if (incomingIt != incomingMap.end())
            {
                const auto& incoming = incomingIt->second;

                if (existingProc.CpuUsage() != incoming.CpuUsage())
                    existingProc.CpuUsage(incoming.CpuUsage());

                if (existingProc.IoRate() != incoming.IoRate())
                    existingProc.IoRate(incoming.IoRate());

                if (existingProc.PrivateBytes() != incoming.PrivateBytes())
                    existingProc.PrivateBytes(incoming.PrivateBytes());

                incomingMap.erase(incomingIt);
                ++it;
            }
            else
            {
                uint32_t index;
                if (Processes().IndexOf(existingProc, index))
                {
                    Processes().RemoveAt(index);
                }
                it = m_uiMap.erase(it);
            }
        }

        for (auto& [pid, newProc] : incomingMap)
        {
            m_uiMap.emplace(pid, newProc);
            Processes().Append(newProc);
        }
    }
}
