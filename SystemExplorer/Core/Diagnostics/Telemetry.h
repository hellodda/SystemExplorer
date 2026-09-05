#pragma once
#include <wil/result.h>

void __stdcall TelemetryCallback(
	_In_ wil::FailureInfo const& failure
) noexcept;