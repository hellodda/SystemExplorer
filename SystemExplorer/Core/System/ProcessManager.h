#pragma once
#include "System.h"

namespace winrt::SystemExplorer::Core::System
{
	using namespace winrt::SystemExplorer::Core::System::Contracts;

	struct ProcessManager : IProcessManager
	{
		void Terminate(uint32_t pid) override;
		void EnableEfficiencyMode(uint32_t pid) override;
		void DisableEfficiencyMode(uint32_t pid) override;
	};
}