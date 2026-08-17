#pragma once

namespace winrt::SystemExplorer::Core::Services
{
	struct AppMemoryManager final
	{
		static void LoadMemoryOptions();
		
		static void SetDefaultMemoryMode();
		static void SetFastMemoryMode();
		static void SetOptimizedMemoryMode();
	};
}
