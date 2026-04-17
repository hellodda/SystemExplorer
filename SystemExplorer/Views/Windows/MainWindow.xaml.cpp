#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("Views/Windows/MainWindow.g.cpp")
#include "Views/Windows/MainWindow.g.cpp"
#endif
#include <Core/Services/AppResourcesService.h>

#pragma comment(lib, "Comctl32.lib")
#include <windowsx.h>
#include <commctrl.h>
#include <iostream>
#include <Helpers/EnumHelper.h>

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

void BindStdHandlesToConsole()
{
    if (!AllocConsole())
    {
        return; 
    }

    FILE* fpDummy;
    freopen_s(&fpDummy, "CONOUT$", "w", stdout);
    freopen_s(&fpDummy, "CONOUT$", "w", stderr);
    freopen_s(&fpDummy, "CONIN$", "r", stdin);

    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // базовый цвет
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    std::cout << "=====================================================================\n";

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "   System Explorer :: Internal Debug Console\n";

    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "   Version: 0.1.0-dev   |   Mode: INTERNAL   |   Session: ACTIVE\n";

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << "---------------------------------------------------------------------\n";

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "[ OK ] ";

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << "Console subsystem initialized\n";

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "[ OK ] ";

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << "Standard streams redirected\n";

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "[ OK ] ";

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << "UTF-8 encoding enabled\n";

    std::cout << "---------------------------------------------------------------------\n";

    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::cout << "[ INFO ] ";

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << "Type 'help' to list available commands\n";

    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::cout << "[ INFO ] ";

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << "Console ready for input\n";

    std::cout << "=====================================================================\n\n";

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

#define WM_HOTKEY_OPEN_CONSOLE (WM_APP + 1)

LRESULT CALLBACK KeyWindowProc(
    _In_ HWND hWnd,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam,
    _In_ UINT_PTR uIdSubclass,
    _In_ DWORD_PTR dwRefData
)
{
    if (uMsg == WM_HOTKEY)
    {
        if (wParam == WM_HOTKEY_OPEN_CONSOLE)
        {
            BindStdHandlesToConsole();
        }
    }
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

namespace winrt::SystemExplorer::Views::Windows::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();

        auto hWnd = reinterpret_cast<HWND>(Hwnd());

		SetWindowSubclass(hWnd, KeyWindowProc, 0, 0);

		RegisterHotKey(hWnd, WM_HOTKEY_OPEN_CONSOLE, MOD_ALT, 'C');

        ExtendsContentIntoTitleBar(true);
        AppWindow().TitleBar().PreferredHeightOption(TitleBarHeightOption::Tall);

        Core::Settings::UserSettings::Instance().AppearanceSettings().SettingChanged([this](auto& sender, auto& args) {
            this->OnSystemBackdropSettingChanged(sender, args);
            this->OnApplicationThemeSettingChanged(sender, args);
        });

        LoadSettings();
    }

    winrt::Microsoft::UI::Xaml::Media::SystemBackdrop GetBackdrop(Core::Data::Enums::BackdropMaterialType material)
    {
        switch (material)
        {
        case Core::Data::Enums::BackdropMaterialType::Static: return nullptr;
        case Core::Data::Enums::BackdropMaterialType::Mica: return Microsoft::UI::Xaml::Media::MicaBackdrop{};
        case Core::Data::Enums::BackdropMaterialType::MicaAlt:
        {
            auto backdrop = Microsoft::UI::Xaml::Media::MicaBackdrop{};
            backdrop.Kind(Microsoft::UI::Composition::SystemBackdrops::MicaKind::BaseAlt);

            return backdrop;
        }
        case Core::Data::Enums::BackdropMaterialType::Acrylic: return Microsoft::UI::Xaml::Media::DesktopAcrylicBackdrop{};
        case Core::Data::Enums::BackdropMaterialType::ThinAcrylic: return Microsoft::UI::Xaml::Media::DesktopAcrylicBackdrop{};
        default: return nullptr;
        }
    }


    void MainWindow::LoadSettings()
    {
        auto appearanceSettings = Core::Settings::UserSettings::Instance().AppearanceSettings();

        this->SystemBackdrop(GetBackdrop(appearanceSettings.BackdropMaterial()));
        this->RootGrid().RequestedTheme(appearanceSettings.ApplicationTheme());

        winrt::SystemExplorer::Core::Services::AppResourcesService::Instance().SetAppThemeBackgroundColor(XamlToolkit::WinUI::Helpers::ColorHelper::ColorHelper::ToColor(appearanceSettings.AppThemeBackgroundColor()));

    }
    void MainWindow::OnSystemBackdropSettingChanged(IInspectable const& sender, Core::Data::EventArguments::SettingChangedEventArgs const& args)
	{
        if (args.SettingName() == L"BackdropMaterial")
        {
            auto type = unbox_value<Core::Data::Enums::BackdropMaterialType>(args.NewValue());
            auto typeStr = SystemExplorer::Helpers::EnumHelper::Map(type);

            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

            // [SETTING]
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            std::cout << "[SETTING] ";

            // ключ
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cout << "BackdropMaterial ";

            // стрелка
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << "-> ";

            // значение
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            std::cout << to_string(typeStr) << "\n";

            // вернуть стандартный цвет
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            this->SystemBackdrop(GetBackdrop(type));
        }
    }
    void MainWindow::OnApplicationThemeSettingChanged(IInspectable const& sender, Core::Data::EventArguments::SettingChangedEventArgs const& args)
    {
        if (args.SettingName() == L"ApplicationTheme")
        {
            this->RootGrid().RequestedTheme(unbox_value<ElementTheme>(args.NewValue()));
        }
    }
}

