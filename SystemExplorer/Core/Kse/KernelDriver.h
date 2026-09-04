#pragma once


typedef enum _KSE_MESSAGE_ID
{
	KseInvalidMessageId,

	KseMsgGetOsVersion,
	KseMsgOpenProcess,
	KseMsgTerminateProcess,
	KseMsgSuspendProcess,

	KseUnhandledMessageId,
	KseMaxMessageId
} KSE_MESSAGE_ID;

typedef struct _KSEM_GET_OS_VERSION
{
	NTSTATUS Status;
	ULONG Major;
	ULONG Minor;
	ULONG Build;
} KSEM_GET_OS_VERSION, * PKSE_GET_OS_VERSION;

typedef struct _KSEM_OPEN_PROCESS
{
	OUT NTSTATUS Status;
	IN PHANDLE ProcessHandle;
	IN ACCESS_MASK DesiredAccess;
	IN PCLIENT_ID ClientId;
} KSEM_OPEN_PROCESS, * PKSEM_OPEN_PROCESS;

typedef struct _KSEM_TERMINATE_PROCESS
{
	OUT NTSTATUS Status;
	IN HANDLE ProcessHandle;
	IN NTSTATUS ExitStatus;
} KSEM_TERMINATE_PROCESS, * PKSEM_TERMINATE_PROCESS;

typedef struct _KSEM_SUSPEND_PROCESS
{
	OUT NTSTATUS Status;
	IN HANDLE ProcessHandle;
} KSEM_SUSPEND_PROCESS, * PKSEM_SUSPEND_PROCESS;

typedef struct _KSE_MESSAGE
{
	struct
	{
		USHORT Version;
		USHORT Size;
		KSE_MESSAGE_ID MessageId;
		LARGE_INTEGER TimeStamp;
	} Header;

	union
	{
		union
		{
			KSEM_GET_OS_VERSION GetOsVersion;
			KSEM_OPEN_PROCESS OpenProcess;
			KSEM_TERMINATE_PROCESS TerminateProcess;
			KSEM_SUSPEND_PROCESS SuspendProcess;
		} User;

		union
		{
			int a;
		} Kernel;

		union
		{
			int a;
		} Reply;
	};
} KSE_MESSAGE, * PKSE_MESSAGE;

typedef CONST PKSE_MESSAGE PCKSE_MESSAGE;

namespace winrt::SystemExplorer::Core::Kernel
{
	using DriverMessage = KSE_MESSAGE;
	using DriverMessageId = KSE_MESSAGE_ID;

	constexpr void init_message(DriverMessage* message, DriverMessageId messageId)
	{
		RtlZeroMemory(message, sizeof(KSE_MESSAGE));
		message->Header.Version = 1;
		message->Header.Size = sizeof(KSE_MESSAGE);
		message->Header.MessageId = messageId;
	}

	enum struct KernelDriverStatus
	{
		None,
		Connected,
		ConnectedSecured
	};

	struct KernelDriver
	{
		[[nodiscard]] static NTSTATUS Connect();

		[[nodiscard]] static NTSTATUS SendNewMessage(gsl::not_null<DriverMessage*> message);

		[[nodiscard]] static constexpr KernelDriverStatus GetDriverStatus() noexcept
		{
			return status_;
		}

		[[nodiscard]] static constexpr bool IsConnectionValid() noexcept
		{
			return status_ > KernelDriverStatus::None;
		}

	private:
		static inline wil::unique_handle device_{ nullptr };
		static inline KernelDriverStatus status_{ KernelDriverStatus::None };
	};
}

