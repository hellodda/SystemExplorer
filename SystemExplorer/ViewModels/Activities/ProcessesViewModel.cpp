#include "pch.h"
#include "winrt_module_imports.h"
#include "ProcessesViewModel.h"
#if __has_include("ViewModels/Activities/ProcessesViewModel.g.cpp")
#include "ViewModels/Activities/ProcessesViewModel.g.cpp"
#endif

#include <absl/container/flat_hash_set.h>

#include <core/eil/nt.h>

#include <core/system/controllers/NativeProcessController.h>
#include <core/system/sources/NativeProcessDataSource.h>
#include <core/system/sources/WTSProcessDataSource.h>

#include <App.xaml.h>

namespace winrt::SystemExplorer::ViewModels::Activities::implementation
{
	ProcessesViewModel::ProcessesViewModel()
	{
		monitor_ = std::make_unique<Core::System::Monitors::ProcessMonitor>(Core::System::Monitors::MonitorOptions{});
		controller_ = std::make_unique<Core::System::Controllers::NativeProcessController>();

		controllerAccess_ = controller_->Access();

		LOG_IF_FAILED(monitor_->DataSource(std::make_unique<Core::System::Sources::NativeProcessDataSource>()));
		LOG_IF_FAILED(monitor_->Start());

		winrt::Microsoft::UI::Dispatching::DispatcherQueue dispatcher = winrt::Microsoft::UI::Dispatching::DispatcherQueue::GetForCurrentThread();

		monitor_->OnDataCollected([weak = get_weak(), dispatcher](std::span<SYSX_PROCESS_ITEM*> processes)
			{
				std::vector<SYSX_PROCESS_ITEM> safeProcesses;
				safeProcesses.reserve(processes.size());

				for (auto* p : processes)
				{
					SYSX_PROCESS_ITEM item = *p;

					if (p->ProcessName.Buffer && p->ProcessName.Length > 0)
					{
						size_t charCount = p->ProcessName.Length / sizeof(WCHAR);

						WCHAR* newBuffer = static_cast<WCHAR*>(CoTaskMemAlloc(p->ProcessName.Length + sizeof(WCHAR)));
						if (newBuffer)
						{
							std::memcpy(newBuffer, p->ProcessName.Buffer, p->ProcessName.Length);
							newBuffer[charCount] = L'\0';

							item.ProcessName.Buffer = newBuffer;
							item.ProcessName.MaximumLength = p->ProcessName.Length + sizeof(WCHAR);
						}
					}

					safeProcesses.push_back(item);
				}

				dispatcher.TryEnqueue([weak, data = std::move(safeProcesses)]() mutable
					{
						if (auto wrf = weak.get())
						{
							wrf->collectData(data);
						}
					});
			});
	}

	void ProcessesViewModel::SelectedProcess(ProcessItem const& value) noexcept
	{
		if (value != SelectedProcess_)
		{
			SelectedProcess_ = value;
			TerminateProcessCommand.NotifyCanExecuteChanged();
			EfficiencyModeCommand.NotifyCanExecuteChanged();
		}
	}

	void ProcessesViewModel::collectData(const std::vector<SYSX_PROCESS_ITEM>& data)
	{
		absl::flat_hash_set<uint32_t> currentPids;
		currentPids.reserve(data.size());

		for (const auto& process : data)
		{
			uint32_t pid = static_cast<uint32_t>(reinterpret_cast<ULONG_PTR>(process.ProcessId));
			currentPids.insert(pid);

			auto it = processMap_.find(pid);
			if (it != processMap_.end())
			{
				it->second.CpuUsage(process.CpuUsage);
			}
			else
			{
				winrt::SystemExplorer::Core::Data::Items::ProcessItem newItem;

				newItem.Pid(pid);
				newItem.CpuUsage(250);
				newItem.IoRate(5000000);
				newItem.PrivateBytes(1000000);
				newItem.Name(eil::nt::to_wstring_view(process.ProcessName));

				Processes().Append(newItem);
				processMap_.emplace(pid, newItem);
			}
		}

		for (auto it = processMap_.begin(); it != processMap_.end(); )
		{
			if (!currentPids.contains(it->first))
			{
				uint32_t index;
				if (Processes().IndexOf(it->second, index))
				{
					Processes().RemoveAt(index);
				}

				processMap_.erase(it++);
			}
			else
			{
				++it;
			}
		}

		for (auto& process : data)
		{
			if (process.ProcessName.Buffer)
			{
			}
		}
	}

	winrt::IAsyncAction ProcessesViewModel::showErrorDialogAsync(const hstring& message)
	{
		MessageBox(NULL, message.c_str(), L"Action failed", MB_OK | MB_ICONERROR);
		co_return;
	}
}