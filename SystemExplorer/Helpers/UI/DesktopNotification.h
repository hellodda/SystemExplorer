#pragma once

namespace winrt::SystemExplorer::Helpers::UI
{
    struct DesktopNotificationArgs
    {
        winrt::hstring Title;
        winrt::hstring Message;
        winrt::hstring Tag;
    };

    struct DesktopNotification
    {
        static bool ShouldSendNotification();
        static void SendNotification(DesktopNotificationArgs const& args, std::function<void()> activatedFunc);
    private:
        static inline std::atomic<uint64_t> _lastNotificationTime{ 0 };
        static constexpr uint64_t MinNotificationIntervalMs{ 5'000 };
    };
}