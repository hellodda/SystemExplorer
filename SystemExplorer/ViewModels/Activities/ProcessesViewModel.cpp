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

		for (auto item : data)
		{
			auto process = Core::Data::Items::ProcessItem{};
			process.Name(eil::nt::to_wstring(item->ProcessName));
			process.CpuUsage(25);
			process.IoRate(50);
			process.PrivateBytes(100);

			Processes.Append(process);
		}

		monitor_->Suspend();
	}

	winrt::IAsyncAction ProcessesViewModel::showErrorDialogAsync(const hstring& message)
	{
		MessageBox(NULL, message.c_str(), L"Action failed", MB_OK | MB_ICONERROR);
		co_return;
	}
}