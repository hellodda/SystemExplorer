#include "pch.h"
#include "DesktopNotification.h"
#include "../SeExeUtils.h"
#include <shobjidl.h>
#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.Data.Xml.Dom.h>

using namespace winrt::Windows::UI::Notifications;
using namespace winrt::Windows::Data::Xml::Dom;

namespace winrt::SystemExplorer::Helpers::UI
{
    bool DesktopNotification::ShouldSendNotification()
    {
        const auto now = GetTickCount64();
        auto last = _lastNotificationTime.load(std::memory_order_relaxed);

        if (now - last < MinNotificationIntervalMs)
        {
            return false;
        }
        return _lastNotificationTime.compare_exchange_strong(last, now, std::memory_order_relaxed);
    }

    void DesktopNotification::SendNotification(DesktopNotificationArgs const& args, std::function<void()> activatedFunc)
    {
        try
        {
            if (!ShouldSendNotification())
            {
                return;
            }

            auto toastXml = ToastNotificationManager::GetTemplateContent(ToastTemplateType::ToastText02);

            auto textNodes = toastXml.GetElementsByTagName(L"text");
            textNodes.Item(0).InnerText(args.Title);
            textNodes.Item(1).InnerText(args.Message);

            auto toastElement = toastXml.DocumentElement();
            // na budusee
            toastElement.SetAttribute(L"launch", L"--from-toast");
            toastElement.SetAttribute(L"scenario", L"default");

            auto toast = ToastNotification{ toastXml };

            toast.Tag(args.Tag);
            toast.Group(L"SystemExplorer");

            if (activatedFunc)
            {
                toast.Activated([activatedFunc](const auto& /*sender*/, const auto& /*eventArgs*/) {
                    activatedFunc();
                });
            }

            winrt::Windows::UI::Notifications::ToastNotifier notifier{ nullptr };
            if (IsPackaged())
            {
                notifier = ToastNotificationManager::CreateToastNotifier();
            }
            else
            {
                wil::unique_cotaskmem_string aumid;
                if (SUCCEEDED(GetCurrentProcessExplicitAppUserModelID(&aumid)))
                {
                    notifier = ToastNotificationManager::CreateToastNotifier(aumid.get());
                }
            }
            if (notifier)
            {
                notifier.Show(toast);
            }
        }
        catch (...)
        {
            LOG_CAUGHT_EXCEPTION();
        }
    }
}