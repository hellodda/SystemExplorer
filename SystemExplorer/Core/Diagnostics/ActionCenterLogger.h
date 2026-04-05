#pragma once
#include <windows.h>
#include <wil/result.h>

namespace winrt::SystemExplorer::Core::Diagnostics
{
	struct ActionsCenterLogger
	{
		static void LogInformation(std::wstring const& message)
		{

		}
		static void LogError(std::wstring const& message)
		{

		}
	private:
		//SystemExplorer::ViewModels::Flyouts::ActionCenterViewModel viewModel_;
	};

	void __stdcall WilFailureCallback(wil::FailureInfo const& failure) noexcept
	{
		wchar_t message[2048];
		message[0] = L'\0';
		wil::GetFailureLogString(message, ARRAYSIZE(message), failure);

		ActionsCenterLogger::LogInformation(message);
	}
}

