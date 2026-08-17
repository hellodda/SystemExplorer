#include "pch.h"
#include "MonitorBase.h"

namespace winrt::SystemExplorer::Core::System::Monitors
{
    HRESULT MonitorBase::Start() noexcept
    {
        MonitorState expected = MonitorState::Stopped;
        if (!state_.compare_exchange_strong(expected,
            options_.StartSuspended ? MonitorState::Suspended : MonitorState::Running))
        {
            return S_FALSE;
        }

        try
        {
            OnStart();

            monitorThread_ = std::jthread([this](std::stop_token stoken)
            {
                monitorRoutine(std::move(stoken));
            });
        }
        catch (...)
        {
            state_ = MonitorState::Stopped;
            return E_FAIL;
        }

        return S_OK;
    }

    HRESULT MonitorBase::Suspend() noexcept
    {
        MonitorState expected = MonitorState::Running;
        if (!state_.compare_exchange_strong(expected, MonitorState::Suspended))
            return S_FALSE; 

        OnSuspend();

        cv_.notify_all();

        return S_OK;
    }

    HRESULT MonitorBase::Resume() noexcept
    {
        MonitorState expected = MonitorState::Suspended;
        if (!state_.compare_exchange_strong(expected, MonitorState::Running))
            return S_FALSE; 

        OnResume();

        cv_.notify_all();

        return S_OK;
    }

    HRESULT MonitorBase::Stop() noexcept
    {
        MonitorState oldState = state_.exchange(MonitorState::Stopped);
        if (oldState == MonitorState::Stopped)
            return S_FALSE;

        OnStop();

        if (monitorThread_.joinable())
        {
            monitorThread_.request_stop();

            if (monitorThread_.get_id() != std::this_thread::get_id())
            {
                monitorThread_ = {}; 
            }
        }

        return S_OK;
    }

    void MonitorBase::monitorRoutine(std::stop_token stoken)
    {
        while (!stoken.stop_requested())
        {
            std::unique_lock lock(mutex_);
            auto interval = options_.UpdateInterval;

            cv_.wait_for(lock, stoken, interval, [this]()
            {
                return state_ != MonitorState::Running; 
            });

            if (stoken.stop_requested())
                break;

            if (state_ == MonitorState::Suspended)
            {
                cv_.wait(lock, stoken, [this]()
                {
                    return state_ != MonitorState::Suspended;
                });
            }

            if (stoken.stop_requested())
                break;

            lock.unlock();

            if (state_ == MonitorState::Running)
            {
                OnTimer();
            }
        }
    }
}