#pragma once

typedef enum _KSE_MESSAGE_ID
{
	KseInvalidMessageId,

	KseMessageGetOsVersion,

	KseMaxMessageId
} KSE_MESSAGE_ID;

typedef struct _KSEM_GET_OS_VERSION
{
	NTSTATUS Status;
	ULONG Major;
	ULONG Minor;
	ULONG Build;
} KSEM_GET_OS_VERSION;

typedef struct _KSE_MESSAGE
{
	struct
	{
		KSE_MESSAGE_ID MessageId;
		ULONG Size;
	} Header;

	union
	{
		KSEM_GET_OS_VERSION GetOsVersion;

	} User;

	union
	{

	} Kernel;
} KSE_MESSAGE, * PKSE_MESSAGE;

namespace winrt::SystemExplorer::Core::Kernel
{
	enum struct KernelDriverStatus
	{
		None,
		Connected,
		ConnectedSecured
	};

	struct KernelDriver
	{
		[[nodiscard]] static NTSTATUS Connect();

		[[nodisacrd]] static KernelDriverStatus GetDriverStatus();

		[[nodiscard]] static NTSTATUS SendNewMessage(_In_ PKSE_MESSAGE message);

	private:
		static inline wil::unique_handle device_{ nullptr };
		static inline KernelDriverStatus status_{ KernelDriverStatus::None };
	};
}

