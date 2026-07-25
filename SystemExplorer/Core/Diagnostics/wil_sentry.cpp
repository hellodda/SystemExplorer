#include "pch.h"
#include "winrt_module_imports.h"

#include <sentry.h>

void WINAPI WilCaptureSentryEvent(wil::FailureInfo const& failure) noexcept
{
	auto event = sentry_value_new_event();
	auto exception = sentry_value_new_exception("Exception", winrt::to_string(failure.pszMessage).c_str());

	sentry_event_add_exception(event, exception);
	sentry_capture_event(event);
}