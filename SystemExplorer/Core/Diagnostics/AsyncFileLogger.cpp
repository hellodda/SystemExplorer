#include "pch.h"
#include "AsyncFileLogger.h"

namespace winrt::SystemExplorer::Core::Diagnostics
{
	AsyncFileLogger::~AsyncFileLogger()
	{
		isInitialized_ = false;

		if (isRunning_)
		{
			isRunning_ = false;
			if (worker_.joinable())
			{
				worker_.request_stop();
				cv_.notify_all();
				worker_.join();
			}
		}
	}
	void AsyncFileLogger::Initialize(std::filesystem::path const& path)
	{
		std::lock_guard lock(mutex_);

		if (isInitialized_)
			return;

		auto fileExists = std::filesystem::exists(path);

		file_.open(
			path,
			std::ios::out |
			std::ios::app |
			std::ios::binary
		);

		if (!file_.is_open()) 
			throw winrt::hresult_error(E_FAIL, L"Failed to open log file.");
		
		if (!fileExists || std::filesystem::file_size(path) == 0)
		{
			const uint16_t bom = 0xFEFF; // UTF-16 LE BOM
			file_.write(reinterpret_cast<const char*>(&bom), sizeof(bom));
			file_.flush();
		}
		isRunning_ = true;
		worker_ = std::jthread([this](std::stop_token token) { workerThread(token); });
		isInitialized_ = true;
	}
	void AsyncFileLogger::Log(std::wstring const&& message)
	{
		if (!isInitialized_)
			return;
		{
			std::lock_guard lock(mutex_);
			logBuffer_.push_back(std::move(message));
		}
		cv_.notify_one();
	}
	void AsyncFileLogger::workerThread(std::stop_token token)
	{
		const std::wstring newline = L"\r\n";
		std::vector<std::wstring> processBuffer;

		while (!token.stop_requested() || !logBuffer_.empty())
		{
			{
				std::unique_lock lock(mutex_);
				
				cv_.wait(lock, token, [this]()
				{
					return !logBuffer_.empty();
				});

				std::swap(logBuffer_, processBuffer);
			}
			if (!processBuffer.empty()) {
				for (const auto& msg : processBuffer)
				{
					
					file_.write(reinterpret_cast<const char*>(msg.data()), msg.size() * sizeof(wchar_t));
					file_.write(reinterpret_cast<const char*>(newline.data()), newline.size() * sizeof(wchar_t));
				}
				file_.flush();
				processBuffer.clear();
			}
		}
	}
}

void __stdcall WilResultLoggingCallBack(wil::FailureInfo const& failure) noexcept
{
	thread_local bool isLogging{ false };

	if (isLogging)
		return;

	isLogging = true;

	try
	{
		auto now = std::chrono::system_clock::now();
		auto time = std::format(L"{:%Y-%m-%d %H:%M:%S}", now);

		std::wstring fileName = NarrowToWide(failure.pszFile ? failure.pszFile : "unknown_file");
		std::wstring functionName = NarrowToWide(failure.pszFunction ? failure.pszFunction : "unknown_function");
		std::wstring moduleName = NarrowToWide(failure.pszModule ? failure.pszModule : "unknown_module");
		std::wstring codeExpr = NarrowToWide(failure.pszCode ? failure.pszCode : "");
		std::wstring callContext = NarrowToWide(failure.pszCallContext ? failure.pszCallContext : "");

		std::wstring_view message = failure.pszMessage ? failure.pszMessage : L"";

		std::wstring logLine = std::format(
			L"[{}] [TID:{:<5}] [{}] [HR: 0x{:08X}] {}!{} ({}:{}) | Code: '{}' | Msg: '{}' | Ctx: '{}'",
			time,
			failure.threadId,
			GetWilFailureTypeString(failure.type),
			static_cast<uint32_t>(failure.hr),
			moduleName,
			functionName,
			fileName,
			failure.uLineNumber,
			codeExpr.empty() ? L"<none>" : codeExpr,
			message.empty() ? L"<none>" : message,
			callContext.empty() ? L"<none>" : callContext
		);

		winrt::SystemExplorer::Core::Diagnostics::AsyncFileLogger::Instance().Log(std::move(logLine));
	}
	catch (...) 
	{

	}

	isLogging = false;
}
