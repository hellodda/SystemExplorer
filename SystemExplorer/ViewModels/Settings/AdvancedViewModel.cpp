#include "pch.h"
#include "winrt_module_imports.h"
#include "AdvancedViewModel.h"
#if __has_include("ViewModels/Settings/AdvancedViewModel.g.cpp")
#include "ViewModels/Settings/AdvancedViewModel.g.cpp"
#endif

#include <helpers/ui/DesktopNotification.h>
#include <core/settings/settings.h>

namespace winrt::SystemExplorer::ViewModels::Settings::implementation
{
    void AdvancedViewModel::UseDriverAsDataSource(bool value) noexcept
    {
        if (value != UseDriverAsDataSource_)
        {
            Core::Settings::UserSettings::AdvancedSettings.UseDriverAsDataSource = value;

            if (value)
                showWarningToastNotification();
        }
    }

    bool AdvancedViewModel::UseDriverAsDataSource() const noexcept
    {
        return Core::Settings::UserSettings::AdvancedSettings.UseDriverAsDataSource();
    }

    void AdvancedViewModel::showWarningToastNotification()
    {   
        Helpers::UI::DesktopNotificationArgs args{};
        args.Message = L"Внимание вы включили экспереметальную функцию, была включена телеметрия в целях диагностики возможных ошибок.";
        args.Title = L"Вы включили эксперементальную функцию";
        args.Tag = L"Warnings";

        Helpers::UI::DesktopNotification::SendNotification(args, nullptr);
    }
}
