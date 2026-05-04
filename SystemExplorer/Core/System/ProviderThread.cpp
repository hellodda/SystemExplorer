#include "pch.h"
#include "ProviderThread.h"

namespace winrt::SystemExplorer::Core::System
{
    ProviderThread::ProviderThread(callback_handler handler, std::chrono::milliseconds interval)
        : handler_(std::move(handler)), interval_(interval), is_running_(interval.count() > 0)
    {
        if (!handler_)
            throw std::invalid_argument("Callback handler cannot be null");
        worker_thread_ = std::jthread([this](std::stop_token st) { threadBody(st); });
    }
    void ProviderThread::Stop()
    {
        worker_thread_.request_stop();
        cv_.notify_all();
    }
    void ProviderThread::SetInterval(std::chrono::milliseconds interval)
    {
        {
            std::lock_guard lock(mutex_);
            interval_ = interval;
        }
        cv_.notify_all(); 
    }
    void ProviderThread::Resume()
    {
        {
            std::lock_guard lock(mutex_);
            is_running_ = true;
        }
        cv_.notify_all();
    }
    void ProviderThread::Suspend()
    {
        {
            std::lock_guard lock(mutex_);
            is_running_ = false;
        }
    }
    void ProviderThread::threadBody(std::stop_token stop_token)
	{
        while (!stop_token.stop_requested())
        {
            std::unique_lock lock(mutex_);

            cv_.wait(lock, stop_token, [this] {
                return is_running_.load();
                });

            if (stop_token.stop_requested()) break;

            auto current_interval = interval_;
            lock.unlock();

            if (handler_)
            {
                handler_();
            }
            if (current_interval.count() > 0)
            {
                std::unique_lock sleep_lock(mutex_);
                cv_.wait_for(sleep_lock, stop_token, current_interval, [] { return false; });
            }
        }
	}
}
