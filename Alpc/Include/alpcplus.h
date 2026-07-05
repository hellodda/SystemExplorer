#pragma once

#include "alpc.h"
#include <wil/win32_helpers.h>
#include <ppl.h>
#include <pplawait.h>
#include <expected>
#include <type_traits>
#include <winrt/base.h>

#define CMD_GET_SERVICE 300
#define CMD_GET_PROCESSES_SERVICE 301
#define CMD_GET_SERVICES_SERVICE 302
#define CMD_GET_XAML_SERVICE 303

#define CMD_PROCESSES_SERVICE_NOT_SUPPORTED 400
#define CMD_SERVICES_SERVICE_NOT_SUPPORTED 401
#define CMD_XAML_SERVICE_NOT_SUPPORTED 402

namespace alpc
{
	struct AlpcResponseMessage
	{
		AlpcResponseMessage() = default;

		template<typename T>
		AlpcResponseMessage(NTSTATUS status, T const& data)
		{
			if (sizeof(T) > DATA_BUFFER_SIZE)
			{
				return;
			}
			rawResponse_.Status = status;
			std::memcpy(rawResponse_.Data, &data, sizeof(T));
		}

		AlpcResponseMessage(ALPC_RESPONSE_MESSAGE const& rawResponse)
			: rawResponse_(rawResponse) {}

		[[nodiscard]] bool IsSuccess() const
		{
			return NT_SUCCESS(rawResponse_.Status);
		}

		[[nodiscard]] NTSTATUS Status() const
		{
			return rawResponse_.Status;
		}

		void Status(NTSTATUS status)
		{
			rawResponse_.Status = status;
		}

		[[nodiscard]] int32_t Id() const
		{
			return rawResponse_.Response;
		}

		void EnsureSuccessStatus()
		{
			THROW_IF_FAILED_MSG(rawResponse_.Status, "Alpc invalid response");
		}

		template<typename To>
		std::expected<To, NTSTATUS> ReadAs() const
		{
			static_assert(std::is_trivially_copyable_v<To>, "Type must be trivially copyable");

			if (sizeof(To) > DATA_BUFFER_SIZE)
			{
				return std::unexpected(STATUS_BUFFER_TOO_SMALL);
			}
			To result;
			std::memcpy(&result, rawResponse_.Data, sizeof(To));
			return result;
		}

		std::wstring_view Stringfy() const
		{
			return std::wstring_view(reinterpret_cast<PCWSTR>(rawResponse_.Data));
		}

	private:
		ALPC_RESPONSE_MESSAGE rawResponse_{};
	};

	struct AlpcRequestMessage
	{
		explicit AlpcRequestMessage(ALPC_REQUEST_MESSAGE const& rawRequest)
			: rawRequest_(rawRequest) {}

		uint32_t Id() const { return rawRequest_.Request; }
		uint32_t Version() const { return rawRequest_.Version; }

		template<typename To>
		std::expected<To, NTSTATUS> ReadAs() const
		{
			static_assert(std::is_trivially_copyable_v<To>, "Type must be trivially copyable");

			if (sizeof(To) > DATA_BUFFER_SIZE)
			{
				return std::unexpected(STATUS_BUFFER_TOO_SMALL);
			}
			To result;
			std::memcpy(&result, rawRequest_.Data, sizeof(To));
			return result;
		}

		std::wstring_view Stringfy() const
		{
			return std::wstring_view(reinterpret_cast<const wchar_t*>(rawRequest_.Data));
		}
	private:
		ALPC_REQUEST_MESSAGE const& rawRequest_;
	};

	struct PluginInformation
	{
		winrt::guid Guid;
		uint32_t Version;

		std::wstring_view Author;
		std::wstring_view Name;
	};

	struct AlpcClient
	{
		AlpcClient() = default;

		AlpcClient(HANDLE portHandle)
			: portHandle_(portHandle) {}

		AlpcClient(const AlpcClient&) = default;
		AlpcClient& operator=(const AlpcClient&) = default;

		AlpcClient(AlpcClient&&) noexcept = default;
		AlpcClient& operator=(AlpcClient&&) noexcept = default;

		template<typename Payload>
		concurrency::task<AlpcResponseMessage> SendCommandAsync(int32_t commandId, Payload const& payload)
		{
			return concurrency::create_task([this, commandId, payload]() mutable -> AlpcResponseMessage {
				return AlpcSendCommand(portHandle_, commandId, (PVOID)&payload, sizeof(Payload));
			});
		}
		concurrency::task<AlpcResponseMessage> SendCommandAsync(int32_t commandId, std::wstring_view payload)
		{
			return concurrency::create_task([this, commandId, str = std::wstring(payload)]() mutable -> AlpcResponseMessage {
				auto byteSize = str.size() * sizeof(wchar_t);

				return AlpcSendCommand(portHandle_, commandId, reinterpret_cast<PVOID>(str.data()), static_cast<ULONG>(byteSize));
			});
		}
	protected:
		HANDLE portHandle_{ nullptr };
		std::wstring portName_{};
	};

	struct PluginClient;

	struct ProcessesService : private AlpcClient
	{
	public:
		ProcessesService() = default;

		concurrency::task<AlpcResponseMessage> TerminateProcessAsync(uint32_t pid)
		{
			return SendCommandAsync(ALPC_CMD_TERMINATE_PROCESS, pid);
		}

		concurrency::task<AlpcResponseMessage> OpenProcessAsync(std::wstring_view name)
		{
			return SendCommandAsync(ALPC_CMD_OPEN_PROCESS, name);
		}

		friend struct PluginClient;

	private:
		ProcessesService(HANDLE portHandle)
			: AlpcClient(portHandle) {};
	};

	struct PluginClient : AlpcClient
	{
		PluginClient() = default;

		PluginClient(PluginClient&&) noexcept = default;
		PluginClient& operator=(PluginClient&&) noexcept = default;

		PluginClient(const PluginClient&) = delete;
		PluginClient& operator=(const PluginClient&) = delete;

		PluginClient(PluginInformation const& information)
		{
			PLUGIN_INFORMATION rawInformation;
			RtlSecureZeroMemory(&rawInformation, sizeof(rawInformation));

			size_t authorLen = (std::min)(information.Author.size(), static_cast<size_t>(255));
			wcsncpy_s(rawInformation.Author, ARRAYSIZE(rawInformation.Author), information.Author.data(), authorLen);

			size_t nameLen = (std::min)(information.Name.size(), static_cast<size_t>(255));
			wcsncpy_s(rawInformation.Name, ARRAYSIZE(rawInformation.Name), information.Name.data(), nameLen);

			rawInformation.Version = information.Version;
			rawInformation.Guid = information.Guid;

			THROW_IF_FAILED_MSG(
				AlpcPluginStartup(ownedPortHandle_.put(), &rawInformation),
				"failed to connect system explorer"
			);
			portHandle_ = ownedPortHandle_.get();
		}

		concurrency::task<ProcessesService> CreateProcessesServiceAsync()
		{
			auto response = co_await SendCommandAsync(ALPC_CMD_GET_PROCESSES_SERVICE, NULL);

			response.EnsureSuccessStatus();

			co_return ProcessesService(portHandle_);
		}

	private:
		wil::unique_handle ownedPortHandle_{ nullptr };
	};
}