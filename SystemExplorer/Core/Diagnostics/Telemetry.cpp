#pragma once
#include "pch.h"
#include "Telemetry.h"

void __stdcall TelemetryCallback(
	_In_ wil::FailureInfo const& failure
) noexcept
{
	thread_local bool isHandling{ false };

	if (isHandling)
		return;

	isHandling = true;

	try
	{

	}
	catch (...)
	{

	}

	isHandling = false;
}
