#pragma once

namespace winrt::SystemExplorer::Core::System
{
    using callback_handler = std::function<void()>;

    struct ProviderThread
    {
        ProviderThread(
            callback_handler handler,
            std::chrono::milliseconds interval = std::chrono::milliseconds(0)
        );

        void Stop();
        void SetInterval(std::chrono::milliseconds interval);
        void Resume();
        void Suspend();
    private:
        void threadBody(std::stop_token stop_token);
    private:
        callback_handler handler_;
        std::mutex mutex_;
        std::condition_variable_any cv_;
        std::chrono::milliseconds interval_;
        std::atomic<bool> is_running_{ false };
        std::jthread worker_thread_;
    };
}
