#pragma once
#include <string>

namespace winrt::SystemExplorer::Core::Diagnostics::Contracts
{
	__interface ILogger
	{
		void Log(std::wstring const&& message);
	};
}