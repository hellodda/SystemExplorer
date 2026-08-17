#include "pch.h"
#include "NativeProcessDataSource.h"

namespace winrt::SystemExplorer::Core::System::Sources
{
	HRESULT NativeProcessDataSource::Enum(
		std::vector<SYSX_PROCESS_ITEM>& source
	)
	{
		if (!buffer_)
		{
			bufferSize_ = 256 * 1024; 
			buffer_ = std::make_unique_for_overwrite<std::byte[]>(bufferSize_);
		}

		for (auto attempts = 0; attempts < 10; ++attempts)
		{

			ULONG returnLength{ 0 };
			auto status = NtQuerySystemInformation(
				SystemProcessInformation,
				buffer_.get(),
				bufferSize_,
				&returnLength
			);

			if 
			(
				status == STATUS_INFO_LENGTH_MISMATCH ||
				status == STATUS_BUFFER_OVERFLOW ||
				status == STATUS_BUFFER_TOO_SMALL
			)
			{
				bufferSize_ = (returnLength > bufferSize_) ? returnLength + 2048 : bufferSize_ * 2;

				if (bufferSize_ > 12'800'000) [[unlikely]]
					return E_OUTOFMEMORY;

				buffer_ = std::make_unique_for_overwrite<std::byte[]>(bufferSize_);
				continue;
			}

			if (status < 0) [[unlikely]]
				return HRESULT_FROM_NT(status);

			source.clear();
			
			auto* head = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(buffer_.get());

			while (true)
			{
				auto& item = source.emplace_back();

				fillProcessItem(&item, head);

				if (head->NextEntryOffset == 0)
					break;

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

		if (auto status = SYSX_IMPL_OpenProcess(&item->QueryHandle, PROCESS_QUERY_INFORMATION, item->ProcessId); status > 0)
		{
			
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
		item->ProcessId = information->UniqueProcessId;
		item->ParentProcessId = information->InheritedFromUniqueProcessId;
		item->CreateTime = information->CreateTime;
		item->SessionId = information->SessionId;
		item->BasePriority = information->BasePriority;
		
	/*	SYSX_IMPL_OpenProcess(
			&item->QueryHandle,
			PROCESS_QUERY_INFORMATION,
			item->ProcessId
		);*/

		// TODO fill handle required feilds

		item->IoReadDelta.Value = information->ReadTransferCount.QuadPart;
		item->IoWriteDelta.Value = information->WriteTransferCount.QuadPart;
		item->IoOtherDelta.Value = information->OtherTransferCount.QuadPart;
		item->IoReadCountDelta.Value = information->ReadOperationCount.QuadPart;
		item->IoWriteCountDelta.Value = information->WriteOperationCount.QuadPart;
		item->CpuKernelDelta.Value = information->KernelTime.QuadPart;
		item->CpuUserDelta.Value = information->UserTime.QuadPart;

		item->NumberOfThreads = information->NumberOfThreads;
		item->NumberOfHandles = information->HandleCount;
		item->WorkingSetPrivateSize = information->WorkingSetPrivateSize;
		item->PeakNumberOfThreads = std::max(item->PeakNumberOfThreads, information->NumberOfThreads);
		item->VmCounters = *(PVM_COUNTERS_EX)&information->PeakVirtualSize;
		item->IoCounters = (information->ReadOperationCount.QuadPart > 0) ? *(PIO_COUNTERS)&information->ReadOperationCount : IO_COUNTERS{};

		// TODO Update deltas

		item->KernelTime = information->KernelTime;
		item->UserTime = information->UserTime;
	}
}