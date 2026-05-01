#pragma once
#include "System.h"
#include <chrono>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace winrt::SystemExplorer::Core::System
{
    using namespace winrt::SystemExplorer::Core::System::Contracts;

    using callback_handler = std::function<void()>;

    struct ProviderThread : IProviderThread
    {
        explicit ProviderThread(callback_handler handler, std::chrono::milliseconds interval = std::chrono::milliseconds(0))
            : handler_(std::move(handler)), interval_(interval), is_running_(interval.count() > 0)
        {
            if (!handler_)
                throw std::invalid_argument("Callback handler cannot be null");

            // Запускаем поток
            worker_thread_ = std::jthread([this](std::stop_token st) { ThreadLoop(st); });
        }

        // Деструктор не нужен (jthread сам все сделает), 
        // но если нужно явно остановить до удаления объекта:
        void Stop() {
            worker_thread_.request_stop();
            cv_.notify_all();
        }

        void SetInterval(std::chrono::milliseconds interval)
        {
            {
                std::lock_guard lock(mutex_);
                interval_ = interval;
            }
            cv_.notify_all(); // Пробуждаем поток, чтобы применить новый интервал
        }

        void Resume()
        {
            {
                std::lock_guard lock(mutex_);
                is_running_ = true;
            }
            cv_.notify_all();
        }

        void Suspend()
        {
            {
                std::lock_guard lock(mutex_);
                is_running_ = false;
            }
            // Поток заснет на cv_.wait
        }

    private:
        void ThreadLoop(std::stop_token stop_token)
        {
            while (!stop_token.stop_requested())
            {
                std::unique_lock lock(mutex_);

                // Ждем либо сигнала остановки, либо пока станет is_running == true
                cv_.wait(lock, stop_token, [this] {
                    return is_running_.load();
                    });

                if (stop_token.stop_requested()) break;

                // Копируем интервал локально под замком
                auto current_interval = interval_;
                lock.unlock();

                // Выполняем полезную работу
                if (handler_) {
                    handler_();
                }

                // Спим до следующей итерации или до изменения состояния
                if (current_interval.count() > 0) {
                    std::unique_lock sleep_lock(mutex_);
                    cv_.wait_for(sleep_lock, stop_token, current_interval, [] { return false; });
                    // wait_for вернет false по тайм-ауту, либо выйдет раньше при cv_.notify_all()
                }
            }
        }

        callback_handler handler_;

        // Синхронизация
        std::mutex mutex_;
        std::condition_variable_any cv_;

        // Состояние
        std::chrono::milliseconds interval_;
        std::atomic<bool> is_running_{ false };

        std::jthread worker_thread_;
    };
}