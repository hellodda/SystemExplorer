#pragma once
#include <Windows.h>
#include <windef.h>
#include <shared_mutex>

#pragma once

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <list>
#include <functional>
#include <memory>
#include <atomic>
#include <chrono>


namespace winrt::SystemExplorer::Core::System
{
    class ProviderThread;

    class ProviderRegistration : public std::enable_shared_from_this<ProviderRegistration> {
    public:
        using Callback = std::function<void()>;

        ProviderRegistration(ProviderThread* thread, Callback cb);

        void Enable(bool enable = true) { m_enabled = enable; }
        bool IsEnabled() const { return m_enabled; }
        uint32_t GetRunId() const { return m_runId; }

        bool Boost(uint32_t* futureRunId = nullptr);
        void Unregister();

    private:
        friend class ProviderThread;

        ProviderThread* m_thread;
        Callback m_callback;

        std::atomic<bool> m_enabled{ false };
        std::atomic<bool> m_unregistering{ false };
        std::atomic<bool> m_boosting{ false };
        std::atomic<uint32_t> m_runId{ 0 };

        std::shared_mutex m_rundownProtect;
    };

    class ProviderThread {
    public:
        ProviderThread() = default;
        ~ProviderThread();

        ProviderThread(const ProviderThread&) = delete;
        ProviderThread& operator=(const ProviderThread&) = delete;

        void Start(std::chrono::milliseconds interval);
        void Stop();
        void SetInterval(std::chrono::milliseconds interval);

        std::shared_ptr<ProviderRegistration> Register(ProviderRegistration::Callback cb);

    private:
        friend class ProviderRegistration;

        void ThreadRoutine(std::stop_token stoken);
        bool BoostProvider(std::shared_ptr<ProviderRegistration> reg, uint32_t* futureRunId);
        void UnregisterProvider(std::shared_ptr<ProviderRegistration> reg);

        std::chrono::milliseconds m_interval{ 1000 };
        std::jthread m_thread;
        std::mutex m_mutex;
        std::condition_variable_any m_cv;

        std::list<std::shared_ptr<ProviderRegistration>> m_providers;
        std::atomic<int> m_boostCount{ 0 };
    };
}

