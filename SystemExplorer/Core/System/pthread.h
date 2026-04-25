#pragma once
#include "System.h"
#include <functional>
#include <thread>
#include <winrt/base.h>
#include <wil/win32_helpers.h>


//
// budet ubran v sled kommitax tak ka vsyo je jthread lushe budet chem eta huita
//

namespace winrt::SystemExplorer::Core::System
{
    using namespace winrt::SystemExplorer::Core::System::Contracts;

    using callback_handler = std::function<void()>;

    class ProviderThread : public IProviderThread
    {
    public:

        explicit ProviderThread(callback_handler handler, std::chrono::milliseconds interval = std::chrono::milliseconds(0))
            : handler_(std::move(handler)),
            interval_(interval)
        {
            if (!handler_)
                throw winrt::hresult_invalid_argument(L"Callback handler cannot be null");

            timer_.reset(::CreateThreadpoolTimer(TimerCallback, this, nullptr));
            if (!timer_)
                winrt::throw_last_error();

            if (interval_.count() > 0)
            {
                Resume();
            }
        }

        ~ProviderThread()
        {
            if (timer_)
            {
                ::SetThreadpoolTimer(timer_.get(), nullptr, 0, 0);
                ::WaitForThreadpoolTimerCallbacks(timer_.get(), TRUE);
            }
        }

        void SetInterval(std::chrono::milliseconds interval) override
        {
            interval_ = interval;
            if (is_running_)
            {
                Resume();
            }
        }

        void Resume() override
        {
            if (!timer_ || interval_.count() <= 0) return;
            is_running_ = true;
            ScheduleNext(); 
        }

        void Suspend() override
        {
            is_running_ = false;

            if (timer_)
            {
                ::SetThreadpoolTimer(timer_.get(), nullptr, 0, 0);
            }
        }

    private:

        void ScheduleNext()
        {
            if (!timer_ || interval_.count() <= 0) return;

            LARGE_INTEGER liDueTime;
            liDueTime.QuadPart = -static_cast<LONGLONG>(interval_.count() * 10000LL);

            FILETIME ftDueTime;
            ftDueTime.dwLowDateTime = liDueTime.LowPart;
            ftDueTime.dwHighDateTime = liDueTime.HighPart;

            ::SetThreadpoolTimer(timer_.get(), &ftDueTime, 0, 0);
        }


        static void NTAPI TimerCallback(
            _In_ PTP_CALLBACK_INSTANCE,
            _In_ PVOID Context,
            _In_ PTP_TIMER) noexcept
        {
            auto* self = static_cast<ProviderThread*>(Context);
            if (self && self->handler_)
            {
                self->handler_();

                if (self->is_running_)
                {
                    self->ScheduleNext();
                }
            }
        }

        wil::unique_threadpool_timer timer_;
        callback_handler handler_;
        std::chrono::milliseconds interval_;
        bool is_running_ = false;
    };
}