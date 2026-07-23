#include "pch.h"
#include "winrt_module_imports.h"
#include "MainWindow.xaml.h"
#if __has_include("Views/Windows/MainWindow.g.cpp")
#include "Views/Windows/MainWindow.g.cpp"
#endif
#include <Core/Services/AppResourcesService.h>
#include <Helpers/EnumHelper.h>
#include <Helpers/UI/AppSystemBackdrop.h>

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::WinUI3Package;

namespace winrt::SystemExplorer::Views::Windows::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();

        ExtendsContentIntoTitleBar(true);
        AppWindow().TitleBar().PreferredHeightOption(TitleBarHeightOption::Tall);

        Core::Settings::UserSettings::Instance().AppearanceSettings().SettingChanged([this](auto& sender, auto& args) {
            this->OnApplicationThemeSettingChanged(sender, args);
        });

        MenuFlyoutItem restartAsAdminFlyoutItem{};
        restartAsAdminFlyoutItem.Text(L"Restart as admin");

        restartAsAdminFlyoutItem.Click([](auto&, auto&) {
            wchar_t exePath[MAX_PATH];
            if (GetModuleFileNameW(nullptr, exePath, MAX_PATH) == 0)
            {
                return; // Ошибка получения пути
            }


            SHELLEXECUTEINFOW sei = { sizeof(sei) };
            sei.lpVerb = L"runas";     // Ключевой параметр для запроса прав администратора
            sei.lpFile = exePath;
            sei.hwnd = nullptr;        // Можно передать HWND главного окна, чтобы окно UAC было модальным к нему
            sei.nShow = SW_NORMAL;

            // 3. Запускаем новый процесс
            if (ShellExecuteExW(&sei))
            {
                winrt::Microsoft::UI::Xaml::Application::Current().Exit();
            }
            else
            {
                // Ошибка: пользователь мог нажать "Нет" в окне UAC (ERROR_CANCELLED)
                DWORD errorCode = GetLastError();
                if (errorCode == ERROR_CANCELLED)
                {
                    // Обработка отмены (например, показать Flyout или InfoBar)
                }
            }
        });

        FontIcon icon{};
        icon.Glyph(L"\uF305");
        restartAsAdminFlyoutItem.Icon(icon);

        Menu().Items().InsertAt(1, restartAsAdminFlyoutItem);

        LoadSettings();
    }


    void MainWindow::LoadSettings()
    {
        auto appearanceSettings = Core::Settings::UserSettings::Instance().AppearanceSettings();

        this->RootGrid().RequestedTheme(appearanceSettings.ApplicationTheme());

        winrt::SystemExplorer::Core::Services::AppResourcesService::Instance().SetAppThemeBackgroundColor(XamlToolkit::WinUI::Helpers::ColorHelper::ColorHelper::ToColor(appearanceSettings.AppThemeBackgroundColor()));
    }
    void MainWindow::OnApplicationThemeSettingChanged(IInspectable const& sender, Core::Data::EventArguments::SettingChangedEventArgs const& args)
    {
        if (args.SettingName() == L"ApplicationTheme")
        {
            this->RootGrid().RequestedTheme(unbox_value<ElementTheme>(args.NewValue()));
        }
    }
}

