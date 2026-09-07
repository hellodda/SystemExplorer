#pragma once
#include <guiddef.h>
#include <wil/result.h>
#include <TraceLoggingProvider.h>

#define SYSX_TRACELOGGING_PROVIDER_NAME "SystemExplorer"

#define SYSX_TRACELOGGING_PROVIDER g_hSysxTraceLoggingProvider

// {EE58D659-B1F7-454D-83DF-D55823CC3BA1}
#define SYSX_TRACELOGGING_PROVIDER_GUID (0xee58d659, 0xb1f7, 0x454d, \
0x83, 0xdf, 0xd5, 0x58, 0x23, 0xcc, 0x3b, 0xa1)

TRACELOGGING_DECLARE_PROVIDER(
	SYSX_TRACELOGGING_PROVIDER
);

namespace winrt::SystemExplorer::Core::Diagnostics::TraceLogging
{
	void __stdcall TraceLoggingFailFastCallback(
		_In_ wil::FailureInfo const& failure
	) noexcept;

	void __stdcall TraceLoggingMessageResultCallback(
		_Inout_ wil::FailureInfo* pFailure,
		_Inout_updates_opt_z_(cchDebugMessage) PWSTR pszDebugMessage,
		_Pre_satisfies_(cchDebugMessage > 0) size_t cchDebugMessage
	) noexcept;

	void __stdcall TraceLoggingTelemetryCallback(
		_In_ bool alreadyReported,
		_In_ wil::FailureInfo const& failure
	) noexcept;

	inline void SxRegisterTraceLoggingCallbacks() noexcept
	{
		wil::SetFailfastWithContextCallback(TraceLoggingFailFastCallback);
		wil::SetResultMessageCallback(TraceLoggingMessageResultCallback);
		wil::SetResultTelemetryFallback(TraceLoggingTelemetryCallback);
	}

	struct TraceLoggingInitializer
	{
		static constexpr HRESULT Initialize() noexcept
		{
			if (!s_registered)
			{
				SxRegisterTraceLoggingCallbacks();
				s_registered = true;

				return TraceLoggingRegister(SYSX_TRACELOGGING_PROVIDER);
			}
			return S_FALSE;
		}

		static constexpr HRESULT Uninitialize() noexcept
		{
			if (s_registered)
			{
				TraceLoggingUnregister(SYSX_TRACELOGGING_PROVIDER);
				return S_OK;
			}
			return S_FALSE;
		}

	private:
		static inline bool s_registered{ false };
	};
}
