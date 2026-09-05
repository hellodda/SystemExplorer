#include "pch.h"
#include "TraceLogging.h"

#include <core/settings/settings.h>
#include <core/eil/wil.h>
#include <winmeta.h>

TRACELOGGING_DEFINE_PROVIDER(
    SYSX_TRACELOGGING_PROVIDER,
    SYSX_TRACELOGGING_PROVIDER_NAME,
    SYSX_TRACELOGGING_PROVIDER_GUID
);

#define SYSX_TRACELOGGING_FFAST_EVENT_NAME        "SystemExplorer.FailFast"

#define SYSX_TRACELOGGING_RESULT_MESSAGE_EVENT_NAME "SystemExplorer.ResultMessage"

#define SYSX_TRACELOGGING_TELEMETRY_EVENT_NAME    "SystemExplorer.Telemetry"

#define SYSX_TRACELOGGING_KEYWORD_WIL 0x001

namespace winrt::SystemExplorer::Core::Diagnostics::TraceLogging
{
    inline bool IsTraceLoggingEnabled() noexcept
    {
        return true;
    }

    void __stdcall TraceLoggingFailFastCallback(
        _In_ wil::FailureInfo const& failure
    ) noexcept
    {
        thread_local bool handling{ false };

        if (handling || !IsTraceLoggingEnabled())
            return;

        handling = true;

        try
        {
            TraceLoggingWrite(
                SYSX_TRACELOGGING_PROVIDER,
                SYSX_TRACELOGGING_FFAST_EVENT_NAME,
                TraceLoggingLevel(WINEVENT_LEVEL_CRITICAL),
                TraceLoggingKeyword(SYSX_TRACELOGGING_KEYWORD_WIL),
                TraceLoggingHResult(failure.hr, "HRESULT"),
                TraceLoggingHexUInt32(static_cast<UINT32>(failure.status), "NTSTATUS"),
                TraceLoggingString(failure.pszFile, "File"),
                TraceLoggingUInt32(failure.uLineNumber, "Line"),
                TraceLoggingString(failure.pszFunction, "Function"),
                TraceLoggingWideString(failure.pszMessage, "Message"),
                TraceLoggingString(failure.callContextOriginating.contextName, "OriginatingContextName"),
                TraceLoggingString(failure.callContextCurrent.contextName, "CurrentContextName"),
                TraceLoggingPointer(failure.returnAddress, "ReturnAddress"),
                TraceLoggingPointer(failure.callerReturnAddress, "CallerReturnAddress")
            );
        }
        catch (...)
        {
        }

        handling = false;
    }

    void __stdcall TraceLoggingMessageResultCallback(
        _Inout_ wil::FailureInfo* pFailure,
        _Inout_updates_opt_z_(cchDebugMessage) PWSTR pszDebugMessage,
        _Pre_satisfies_(cchDebugMessage > 0) size_t cchDebugMessage
    ) noexcept
    {
        thread_local bool handling{ false };

        if (handling || !IsTraceLoggingEnabled() || pFailure == nullptr)
            return;

        handling = true;

        try
        {
            if (pszDebugMessage != nullptr && cchDebugMessage > 0)
            {
                pszDebugMessage[cchDebugMessage - 1] = L'\0';
            }

            TraceLoggingWrite(
                SYSX_TRACELOGGING_PROVIDER,
                SYSX_TRACELOGGING_RESULT_MESSAGE_EVENT_NAME,
                TraceLoggingLevel(WINEVENT_LEVEL_VERBOSE),
                TraceLoggingKeyword(SYSX_TRACELOGGING_KEYWORD_WIL),
                TraceLoggingHResult(pFailure->hr, "HRESULT"),
                TraceLoggingString(pFailure->pszFile, "File"),
                TraceLoggingUInt32(pFailure->uLineNumber, "Line"),
                TraceLoggingWideString(pszDebugMessage ? pszDebugMessage : L"", "DebugMessage")
            );
        }
        catch (...)
        {
        }

        handling = false;
    }

    void __stdcall TraceLoggingTelemetryCallback(
        _In_ bool alreadyReported,
        _In_ wil::FailureInfo const& failure
    ) noexcept
    {
        thread_local bool handling{ false };

        if (handling || !IsTraceLoggingEnabled())
            return;

        handling = true;

        try
        {
            TraceLoggingWrite(
                SYSX_TRACELOGGING_PROVIDER,
                SYSX_TRACELOGGING_TELEMETRY_EVENT_NAME,
                TraceLoggingLevel(WINEVENT_LEVEL_ERROR),
                TraceLoggingKeyword(SYSX_TRACELOGGING_KEYWORD_WIL),
                TraceLoggingHResult(failure.hr, "HRESULT"),
                TraceLoggingInt32(failure.failureId, "FailureId"),
                TraceLoggingString(failure.pszFile, "File"),
                TraceLoggingUInt32(failure.uLineNumber, "Line"),
                TraceLoggingPointer(failure.returnAddress, "ReturnAddress"),
                TraceLoggingBool(alreadyReported, "AlreadyReported"),
                TraceLoggingBool(failure.type == wil::FailureType::FailFast, "IsFailFast")
            );
        }
        catch (...)
        {
        }

        handling = false;
    }
}