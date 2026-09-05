#include "pch.h"
#include "NativeProcessDataSource.h"
#include <Core/Eil/nt.h>
#include <Helpers/Win32/ShellHelper.h>

namespace winrt::SystemExplorer::Core::System::Sources
{
	HRESULT NativeProcessDataSource::Enum(
		std::vector<SYSX_PROCESS_ITEM>& source
	)
	{
		uint64_t blanker;
		return Enum(source, blanker);
	}

	HRESULT NativeProcessDataSource::Enum(
		std::vector<SYSX_PROCESS_ITEM>& source,
		uint64_t& outSystemTime
	)
	{
		outSystemTime = eil::get_current_system_time();

		if (!buffer_)
		{
			bufferSize_ = 256 * 1024;
			buffer_ = std::make_unique_for_overwrite<std::byte[]>(bufferSize_);
		}

		for (int attempts = 0; attempts < 10; ++attempts)
		{
			ULONG returnLength{ 0 };
			NTSTATUS status = NtQuerySystemInformation(
				SystemProcessInformation,
				buffer_.get(),
				bufferSize_,
				&returnLength
			);

			if (status == STATUS_INFO_LENGTH_MISMATCH ||
				status == STATUS_BUFFER_OVERFLOW ||
				status == STATUS_BUFFER_TOO_SMALL)
			{
				bufferSize_ = (returnLength > bufferSize_) ? returnLength + 8192 : bufferSize_ * 2;
				if (bufferSize_ > 16'777'216) [[unlikely]] return E_OUTOFMEMORY;

				buffer_ = std::make_unique_for_overwrite<std::byte[]>(bufferSize_);
				continue;
			}

			if (status < 0) [[unlikely]]
				return HRESULT_FROM_NT(status);

			source.clear();
			auto* head = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(buffer_.get());

			while (head)
			{
				auto& item = source.emplace_back();
				fillProcessItem(&item, head);

				if (head->NextEntryOffset == 0) break;
				head = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(
					reinterpret_cast<std::byte*>(head) + head->NextEntryOffset
					);
			}

			return S_OK;
		}

		return E_FAIL;
	}

	HRESULT NativeProcessDataSource::Fill(
		_In_ PSYSX_PROCESS_ITEM item
	) 
	{
		if (!item)
			return E_INVALIDARG;

		if (auto status = SeOpenProcess(&item->QueryHandle, PROCESS_QUERY_INFORMATION, item->ProcessId); status > 0)
		{
			LOG_IF_FAILED(SeGetProcessImageFileNameWin32(item->QueryHandle, &item->FileName));
			item->SmallIconIndex = Helpers::Win32::ShellHelper::GetIconIndex(eil::to_wstring_view(item->FileName).data());
		}
		else
		{
			return HRESULT_FROM_NT(status);
		}
	}

	void NativeProcessDataSource::fillProcessItem(
		_Inout_ PSYSX_PROCESS_ITEM item,
		_In_ PSYSTEM_PROCESS_INFORMATION information
	)
	{
		item->ProcessId = reinterpret_cast<HANDLE>(information->UniqueProcessId);
		item->ParentProcessId = reinterpret_cast<HANDLE>(information->InheritedFromUniqueProcessId);
		item->ProcessName = information->ImageName;
		item->CreateTime = information->CreateTime;
		item->SessionId = information->SessionId;
		item->BasePriority = information->BasePriority;

		item->NumberOfThreads = information->NumberOfThreads;
		item->NumberOfHandles = information->HandleCount;
		item->WorkingSetPrivateSize = information->WorkingSetPrivateSize.QuadPart;
		item->VmCounters = *reinterpret_cast<PVM_COUNTERS_EX>(&information->PeakVirtualSize);

		if (information->ReadOperationCount.QuadPart > 0)
		{
			item->IoCounters = *reinterpret_cast<PIO_COUNTERS>(&information->ReadOperationCount);
		}

		item->KernelTime = information->KernelTime;
		item->UserTime = information->UserTime;

		item->CpuKernelDelta.Value = information->KernelTime.QuadPart;
		item->CpuUserDelta.Value = information->UserTime.QuadPart;

		item->IoReadDelta.Value = information->ReadTransferCount.QuadPart;
		item->IoWriteDelta.Value = information->WriteTransferCount.QuadPart;
		item->IoOtherDelta.Value = information->OtherTransferCount.QuadPart;

		item->IoReadCountDelta.Value = information->ReadOperationCount.QuadPart;
		item->IoWriteCountDelta.Value = information->WriteOperationCount.QuadPart;
		item->IoOtherCountDelta.Value = information->OtherOperationCount.QuadPart;

		item->PageFaultsDelta.Value = information->PageFaultCount;
	}
}