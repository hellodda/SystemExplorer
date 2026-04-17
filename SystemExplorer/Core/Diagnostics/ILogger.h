#pragma once

namespace winrt::SystemExplorer::Core::Diagnostics
{
	struct ILogger
	{
		virtual void Log(std::wstring const&& message) = 0;
	};
}