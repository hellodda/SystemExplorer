#pragma once
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace winrt::SystemExplorer::Core::System::Monitors
{
    struct MonitorOptions
    {
        std::chrono::milliseconds UpdateInterval{ 1000 };
        bool StartSuspended{ false };
    };

    enum class MonitorState
    {
        Stopped,
        Running,
        Suspended
    };

    struct MonitorBase
    {
        MonitorBase() noexcept = default;
        explicit MonitorBase(MonitorOptions const& options) noexcept : options_(options) {}
        virtual ~MonitorBase() { (void)Stop(); }

        [[nodiscard]] HRESULT Start() noexcept;
        [[nodiscard]] HRESULT Suspend() noexcept;
        [[nodiscard]] HRESULT Resume() noexcept; 
        [[nodiscard]] HRESULT Stop() noexcept;

        void Options(MonitorOptions const& options) noexcept
        {
            std::lock_guard lock(mutex_); 
            options_ = options;
        }

    protected:
  
        virtual void OnStart() {}
        virtual void OnSuspend() {}
        virtual void OnResume() {}
        virtual void OnStop() {}
        virtual void OnTimer() {}

        HANDLE monitorThreadHandle = monitorThread_.native_handle();

    private:
        void monitorRoutine(std::stop_token stoken);

    private:
        std::jthread monitorThread_;

        MonitorOptions options_;

        std::atomic<MonitorState> state_{ MonitorState::Stopped };
        std::mutex mutex_;
        std::condition_variable_any cv_; 
    };
}