#include "pch.h"
#include "KernelDriver.h"

#define CTL_KSE_DISPATCH CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

namespace winrt::SystemExplorer::Core::Kernel
{
	NTSTATUS KernelDriver::Connect()
	{
		wil::unique_handle handle{ CreateFileW(
			L"\\\\.\\KERNELEXPLORER",
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		) };

		if (!handle.is_valid())
		{
			status_ = KernelDriverStatus::None;
			return STATUS_INVALID_HANDLE;
		}

		device_ = std::move(handle);
		status_ = KernelDriverStatus::Connected;

		return STATUS_SUCCESS;
	}

	NTSTATUS KernelDriver::SendNewMessage(gsl::not_null<DriverMessage*> message)
	{
		if (status_ == KernelDriverStatus::None || !device_.is_valid())
			return STATUS_DEVICE_NOT_READY;

		static constexpr ULONG ioControlCode{ CTL_KSE_DISPATCH };
		static constexpr ULONG messageSize{ sizeof(KSE_MESSAGE) };
		
		message->Header.Size = messageSize;

		IO_STATUS_BLOCK ioStatusBlock{};
		ULONG bytesReturned{ 0 };

		auto status = NtDeviceIoControlFile(
			device_.get(),
			NULL,
			NULL,
			NULL,
			&ioStatusBlock,
			ioControlCode,
			message,
			messageSize,
			message,
			messageSize
		);

		if (status == STATUS_INVALID_HANDLE ||
			status == STATUS_DEVICE_DOES_NOT_EXIST ||
			status == STATUS_OBJECT_NAME_NOT_FOUND)
		{
			return status;
		}

		if (NT_SUCCESS(status))
		{

		}
		return status;
	}
}