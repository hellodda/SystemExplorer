#pragma once

namespace winrt::SystemExplorer::Helpers::Win32::Native
{
	using namespace winrt::Windows::Foundation;


	struct NativeSystem
	{
		struct Kernel
		{
			static IAsyncActionWithProgress<int32_t> CreateLiveKernelMemoryDumpAsync(std::wstring const& filePath);
		};
	};
}