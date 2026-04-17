#pragma once
#include <filesystem>
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <format>
#include <vector>
#include <string>
#include <chrono>
#include <Helpers/Common.h>
#include "ILogger.h"



namespace winrt::SystemExplorer::Core::Diagnostics
{
	struct AsyncFileLogger : ILogger
	{
		~AsyncFileLogger();

		static AsyncFileLogger& Instance()
		{
			static AsyncFileLogger instance;
			return instance;
		}

		void Initialize(std::filesystem::path const& path);
		void Log(std::wstring const&& message) override;

	private:
		AsyncFileLogger() = default;

		void workerThread(std::stop_token token);
	private:
		std::atomic<bool> isInitialized_{ false };
		std::atomic<bool> isRunning_{ false };
		std::ofstream file_;

		std::mutex mutex_;
		std::condition_variable_any cv_;
		std::vector<std::wstring> logBuffer_;
		std::jthread worker_;
	};
}


void WINAPI WilResultLoggingCallBack(wil::FailureInfo const& failure) noexcept;