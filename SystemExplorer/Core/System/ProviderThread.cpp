#include "pch.h"
#include "ProviderThread.h"

namespace winrt::SystemExplorer::Core::System
{
    ProviderRegistration::ProviderRegistration(ProviderThread* thread, Callback cb)
        : m_thread(thread), m_callback(std::move(cb)) {}

    bool ProviderRegistration::Boost(uint32_t* futureRunId) {
        return m_thread->BoostProvider(shared_from_this(), futureRunId);
    }

    void ProviderRegistration::Unregister() {
        m_thread->UnregisterProvider(shared_from_this());
    }


    ProviderThread::~ProviderThread() {
        Stop();
    }

    void ProviderThread::Start(std::chrono::milliseconds interval) {
        if (m_thread.joinable()) return;

        m_interval = interval;
        m_thread = std::jthread([this](std::stop_token stoken) {
            ThreadRoutine(stoken);
            });
    }

    void ProviderThread::Stop() {
        if (m_thread.joinable()) {
            m_thread.request_stop();
            m_cv.notify_all();
            m_thread.join();
        }
    }

    void ProviderThread::SetInterval(std::chrono::milliseconds interval) {
        std::scoped_lock lock(m_mutex);
        m_interval = interval;
        m_cv.notify_one();
    }

    std::shared_ptr<ProviderRegistration> ProviderThread::Register(ProviderRegistration::Callback cb) {
        auto reg = std::make_shared<ProviderRegistration>(this, std::move(cb));

        std::scoped_lock lock(m_mutex);
        m_providers.push_back(reg);
        return reg;
    }

    void ProviderThread::ThreadRoutine(std::stop_token stoken) {
        while (!stoken.stop_requested()) {
            std::list<std::shared_ptr<ProviderRegistration>> tempList;
            std::unique_lock lock(m_mutex);

            m_cv.wait_for(lock, stoken, m_interval, [this]() {
                return m_boostCount > 0;
                });

            if (stoken.stop_requested()) break;

            while (!m_providers.empty()) {
                auto reg = m_providers.front();
                m_providers.pop_front();
                tempList.push_back(reg);

                if (!reg->m_enabled && !reg->m_boosting) continue;
                if (reg->m_unregistering) continue;

                if (reg->m_boosting) {
                    reg->m_boosting = false;
                    m_boostCount--;
                }

                reg->m_runId++;

                lock.unlock();

                {
                    std::shared_lock rundownLock(reg->m_rundownProtect, std::defer_lock);
                    if (rundownLock.try_lock()) {
                        reg->m_callback();
                    }
                }

                lock.lock();
            }

            for (auto& reg : tempList) {
                if (reg->m_boosting) {
                    m_providers.push_front(reg);
                }
                else {
                    m_providers.push_back(reg);
                }
            }
        }
    }

    bool ProviderThread::BoostProvider(std::shared_ptr<ProviderRegistration> reg, uint32_t* futureRunId) {
        std::scoped_lock lock(m_mutex);

        if (reg->m_unregistering || reg->m_boosting || !m_thread.joinable()) {
            return false;
        }

        m_providers.remove(reg);
        m_providers.push_front(reg);

        reg->m_boosting = true;
        m_boostCount++;

        if (futureRunId) {
            *futureRunId = reg->m_runId + 1;
        }
        
        m_cv.notify_one();
        return true;
    }

    void ProviderThread::UnregisterProvider(std::shared_ptr<ProviderRegistration> reg) {
        reg->m_unregistering = true;

        {
            std::scoped_lock lock(m_mutex);
            if (reg->m_boosting) {
                reg->m_boosting = false;
                m_boostCount--;
            }
            m_providers.remove(reg);
        }

        std::unique_lock rundownLock(reg->m_rundownProtect);
    }
}
