#include "pch.h"
#include "winrt_module_imports.h"
#include "ProcessesViewModel.h"
#if __has_include("ViewModels/Activities/ProcessesViewModel.g.cpp")
#include "ViewModels/Activities/ProcessesViewModel.g.cpp"
#endif

#include <absl/container/flat_hash_set.h>

#include <converters/native/HiconToBitmapSourceConverter.h>
#include <Helpers/Win32/ShellHelper.h>
#include <core/eil/nt.h>
#include <core/system/controllers/NativeProcessController.h>
#include <core/system/sources/NativeProcessDataSource.h>
#include <core/system/sources/WTSProcessDataSource.h>
#include <core/system/controllers/WTSProcessController.h>

#include <App.xaml.h>

namespace winrt::SystemExplorer::ViewModels::Activities::implementation
{
	ProcessesViewModel::ProcessesViewModel()
	{
		queue_ = winrt::Microsoft::UI::Dispatching::DispatcherQueue::GetForCurrentThread();

		monitor_ = std::make_unique<Core::System::Monitors::ProcessMonitor>(Core::System::Monitors::MonitorOptions{});
		controller_ = std::make_unique<Core::System::Controllers::NativeProcessController>();

		controllerAccess_ = controller_->Access();

		LOG_IF_FAILED(monitor_->DataSource(std::make_unique<Core::System::Sources::NativeProcessDataSource>()));
		LOG_IF_FAILED(monitor_->Start());

		Core::Messaging::ViewModelsGlobalMessenger.register_handler([weak = get_weak()](Core::Messaging::DataSourceChangedMessage const& messsage) 
		{
			if (auto wrf = weak.get())
				wrf->changeDataSource(messsage);
		});

		monitor_->OnDataCollected([this](std::span<PSYSX_PROCESS_ITEM> data) -> winrt::fire_and_forget
		{
			std::vector<PSYSX_PROCESS_ITEM> safeData(data.begin(), data.end());
			co_await collectDataAsync(safeData);
		});
	}

	void ProcessesViewModel::SelectedProcess(ProcessItem value) noexcept
	{
		if (value != SelectedProcess_)
		{
			SelectedProcess_ = std::move(value);
			TerminateProcessCommand.NotifyCanExecuteChanged();
			EfficiencyModeCommand.NotifyCanExecuteChanged();
		}
	}

	void ProcessesViewModel::changeDataSource(Core::Messaging::DataSourceChangedMessage const& message)
	{
		
	}

	winrt::IAsyncAction ProcessesViewModel::collectDataAsync(std::vector<PSYSX_PROCESS_ITEM> const& data)
	{
		auto strong_this = get_strong();

		co_await wil::resume_foreground(queue_);

		absl::flat_hash_set<uint32_t> currentPids;
		int32_t activeProcesses = 0;
		double totalCpuUsage = 0.0;
		uint64_t totalIoRate = 0;
		uint64_t totalPrivateBytes = 0;

		currentPids.reserve(data.size());

		for (const auto item : data)
		{
			uint32_t pid = static_cast<uint32_t>(reinterpret_cast<ULONG_PTR>(item->ProcessId));
			currentPids.insert(pid);

			totalCpuUsage += item->CpuUsage;

			totalIoRate +=
				static_cast<uint64_t>(item->IoReadDelta.Delta) +
				static_cast<uint64_t>(item->IoWriteDelta.Delta);

			totalPrivateBytes +=
				static_cast<uint64_t>(item->VmCounters.PrivateUsage);

			auto it = processMap_.find(pid);
			if (it != processMap_.end())
			{
				auto& process = it->second;
				process.CpuUsage(item->CpuUsage);
				process.IoRate(static_cast<uint32_t>(item->IoReadDelta.Delta + item->IoWriteDelta.Delta));
				process.PrivateBytes(static_cast<uint32_t>(item->VmCounters.PrivateUsage));
			}
			else
			{
				auto process = Core::Data::Items::ProcessItem{};

				process.Pid(pid);
				process.Handle(reinterpret_cast<uint64_t>(item->QueryHandle));
				process.ParentId(static_cast<uint32_t>(reinterpret_cast<ULONG_PTR>(item->ParentProcessId)));
				process.Name(eil::to_wstring_view(item->ProcessName));

				if (item->FileName.Buffer)
				{
					process.Description(eil::to_wstring_view(item->FileName));
				}
				else
				{
					process.Description(L"");
				}

				process.Icon(Converters::Native::HiconToBitmapSourceConverter::Convert(
					Helpers::Win32::ShellHelper::GetIconByIndex(Helpers::Win32::ShellHelper::GetDefaultIconIndex())
				));

				process.CpuUsage(item->CpuUsage);
				process.IoRate(static_cast<uint32_t>(item->IoReadDelta.Delta + item->IoWriteDelta.Delta));
				process.PrivateBytes(static_cast<uint32_t>(item->VmCounters.PrivateUsage));

				processMap_.emplace(pid, process);
				Processes.Append(process);
			}
		}
		for (int32_t i = static_cast<int32_t>(Processes.Size()) - 1; i >= 0; --i)
		{
			auto const& process = Processes.GetAt(i);

			if (!currentPids.contains(process.Pid()))
			{
				processMap_.erase(process.Pid());
				Processes.RemoveAt(i);
			}
		
		}
		TotalCpuUsage(totalCpuUsage);
		TotalIoRate(totalIoRate);
		TotalPrivateBytes(totalPrivateBytes);
	}

	winrt::IAsyncAction ProcessesViewModel::showErrorDialogAsync(const hstring& message)
	{
		MessageBox(NULL, message.c_str(), L"Action failed", MB_OK | MB_ICONERROR);
		co_return;
	}
}