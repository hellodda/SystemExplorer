#include "pch.h"
#include "NativeSystem.h"

#include <Core/System/Native.h>
#include <Core/Settings/UserSettings.h>

#include "../../../../Common/thread_pool.h"

using namespace winrt::SystemExplorer::Core::Settings;

namespace winrt::SystemExplorer::Helpers::Win32::Native
{
	uint64_t NativeSystem::GetCurrentSystemTime()
	{
		FILETIME idleTime{}, kernelTime{}, userTime{};

		if (!GetSystemTimes(&idleTime, &kernelTime, &userTime))
			return 0;
		return std::bit_cast<uint64_t>(kernelTime) + std::bit_cast<uint64_t>(userTime);
	}

	///
	/// Kernel actions impl \/ \/ \/
	/// 

	IAsyncActionWithProgress<int32_t> NativeSystem::Kernel::CreateLiveKernelMemoryDumpAsync(std::wstring const& filePath)
	{
		auto cancellationToken = co_await get_cancellation_token();
		auto progress = co_await get_progress_token();

		progress(0);

		co_await resume_background();

		wil::unique_hfile file{ CreateFileW(
			filePath.c_str(),
			GENERIC_WRITE | GENERIC_READ,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		) };

		if (!file.is_valid())
			throw hresult_error(E_FAIL ,L"pure aura");

		wil::unique_event cancelEvent{
			CreateEventW(NULL, TRUE, FALSE, NULL)
		};

		auto hCancelEvent = cancelEvent.get();
		delegate<> cancelDelegate = [hCancelEvent] {
			if (hCancelEvent) SetEvent(hCancelEvent);
		};
		cancellationToken.callback(std::move(cancelDelegate));

		SE_LIVE_DUMP_CONFIG configuration{ 0 };
		configuration.FileName = const_cast<PWSTR>(filePath.c_str());
		configuration.FileHandle = file.get();
		configuration.EventHandle = cancelEvent.get();
		
		auto settings = UserSettings::Instance().GeneralSettings();

		configuration.Options.IncludeHypervisorPages =
			settings.MemoryDumpCaptureHypervisorPages();

		configuration.Options.IncludeNonEssentialHypervisorPages =
			settings.MemoryDumpIncludeNonessentialPages();

		configuration.Options.IncludeUserSpaceMemory =
			settings.MemoryDumpCaptureUserPages();

		native::thread_pool<>::global().enqueue(SepCreateLiveKernelDump, &configuration).wait();

		if (cancellationToken())
			throw hresult_canceled{};

		progress(100);
	}

	
}