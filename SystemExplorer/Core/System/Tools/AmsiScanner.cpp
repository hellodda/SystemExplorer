#include "pch.h"
#include "AmsiScanner.h"

#pragma comment(lib, "Amsi.lib")

namespace winrt::SystemExplorer::Core::System::Tools
{
	unique_amsi_context AmsiScanner::context_{ nullptr };
	std::once_flag amsiInitOnce;

	AmsiScanner::AmsiScanner(hstring const& appName)
	{
		std::call_once(amsiInitOnce, [&]() {
			THROW_IF_FAILED(AmsiInitialize(appName.c_str(), context_.put()));
		});

		THROW_IF_FAILED(AmsiOpenSession(context_.get(), &session_));
	}
	AmsiScanner::~AmsiScanner()
	{
		if (session_ && context_)
		{
			AmsiCloseSession(context_.get(), session_);
		}
	}
	AmsiScanState AmsiScanner::ScanBuffer(PCVOID buffer, uint32_t length, hstring const& contentName)
	{
		if (!buffer || length == 0) return AmsiScanState::NotDetected;

		AMSI_RESULT rawResult{ AMSI_RESULT_NOT_DETECTED };

		THROW_IF_FAILED(AmsiScanBuffer(
			context_.get(),
			const_cast<PVOID>(buffer),
			length,
			contentName.c_str(),
			session_,
			&rawResult
		));

		return parseAmsiResult(rawResult);
	}
	AmsiScanState AmsiScanner::ScanBuffer(std::vector<uint8_t> const& buffer, hstring const& contentName)
	{
		return ScanBuffer(buffer.data(), static_cast<uint32_t>(buffer.size()), contentName);
	}
	AmsiScanState AmsiScanner::parseAmsiResult(AMSI_RESULT result)
	{
		if (AmsiResultIsMalware(result)) return AmsiScanState::Malware;
		if (AmsiResultIsBlockedByAdmin(result)) return AmsiScanState::BlockedByAdmin;
		if (result == AMSI_RESULT_CLEAN) return AmsiScanState::Clean;
		return AmsiScanState::NotDetected;
	}
}