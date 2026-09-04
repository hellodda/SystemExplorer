#include "pch.h"
#include "winrt_module_imports.h"
#include "ProcessPropertiesHelper.h"
#include <Helpers/UI/AppSystemBackdrop.h>
#include <App.xaml.h>
#include <Views/Pages/Properties/PropertiesRootPage.xaml.h>
#include <Views/Pages/Properties/ProcessImagePropertiesPage.xaml.h>
#include <Core/Data/Items/ObjectPairItem.h>

#undef max
#undef min

using namespace winrt::Windows::Graphics;
using namespace winrt::Microsoft::UI::Windowing;
using namespace winrt::WinUI3Package;

namespace winrt::SystemExplorer::Helpers
{
    void ProcessPropertiesHelper::OpenPropertiesWindow(IInspectable const& item)
    {
        auto frame = Frame{};
        frame.RequestedTheme(ElementTheme::Dark);

        auto propertiesWindow = WindowEx{};
        propertiesWindow.Closed(&ProcessPropertiesHelper::PropertiesWindows_Closed);
        propertiesWindow.SystemBackdrop(Helpers::UI::AppSystemBackdrop{ true });
        propertiesWindow.Content(frame);
        propertiesWindow.IsMaximizable(false);
        propertiesWindow.IsMinimizable(false);
        propertiesWindow.Title(StringsHelper::Properties());

        auto width = std::max(1, static_cast<int32_t>(800 * CurrentApplication::AppModel().AppWindowDPI()));
        auto height = std::max(1, static_cast<int32_t>(500 * CurrentApplication::AppModel().AppWindowDPI()));

        auto appWindow = propertiesWindow.AppWindow();
        appWindow.Resize(SizeInt32{ width, height });
        appWindow.TitleBar().ExtendsContentIntoTitleBar(true);
        appWindow.TitleBar().ButtonBackgroundColor(Windows::UI::Colors::Transparent());
        appWindow.TitleBar().ButtonInactiveBackgroundColor(Windows::UI::Colors::Transparent());

        Core::Data::Items::ObjectPairItem ImageMenuItem;
        ImageMenuItem.First(winrt::box_value(L"Image"));
        ImageMenuItem.Second(winrt::box_value(winrt::xaml_typename<winrt::SystemExplorer::Views::Pages::Properties::ProcessImagePropertiesPage>()));

        Core::Data::Items::ObjectPairItem PerformanceMenuItem;
        PerformanceMenuItem.First(winrt::box_value(L"Performance"));
        
        Core::Data::Items::ObjectPairItem PerformanceGraphMenuItem;
        PerformanceGraphMenuItem.First(winrt::box_value(L"Performance Graph"));

        Core::Data::Items::ObjectPairItem GpuGraphMenuItem;
        GpuGraphMenuItem.First(winrt::box_value(L"GPU Graph"));

        Core::Data::Items::ObjectPairItem ThreadsMenuItem;
        ThreadsMenuItem.First(winrt::box_value(L"Threads"));

        Core::Data::Items::ObjectPairItem TcpIpMenuItem;
        TcpIpMenuItem.First(winrt::box_value(L"TCP/IP"));

        Core::Data::Items::ObjectPairItem SecurityMenuItem;
        SecurityMenuItem.First(winrt::box_value(L"Security"));

        Core::Data::Items::ObjectPairItem EnvironmentMenuItem;
        EnvironmentMenuItem.First(winrt::box_value(L"Environment"));

        Core::Data::Items::ObjectPairItem StringsMenuItem;
        StringsMenuItem.First(winrt::box_value(L"Strings"));

        auto propertiesPage = winrt::SystemExplorer::Views::Pages::Properties::PropertiesRootPage{};
        propertiesPage.Properties().Append(ImageMenuItem);
        propertiesPage.Properties().Append(PerformanceMenuItem);
        propertiesPage.Properties().Append(PerformanceMenuItem);
        propertiesPage.Properties().Append(PerformanceGraphMenuItem);
        propertiesPage.Properties().Append(GpuGraphMenuItem);
        propertiesPage.Properties().Append(ThreadsMenuItem);
        propertiesPage.Properties().Append(TcpIpMenuItem);
        propertiesPage.Properties().Append(SecurityMenuItem);
        propertiesPage.Properties().Append(EnvironmentMenuItem);


        frame.Content(propertiesPage);

        POINT pointerPosition;
        if (!GetCursorPos(&pointerPosition))
        {

        }

        auto displayArea = DisplayArea::GetFromPoint(
            PointInt32{ pointerPosition.x, pointerPosition.y },
            DisplayAreaFallback::Nearest
        );
        auto workArea = displayArea.WorkArea();
        auto appWindowPos = PointInt32
        {
            .X = workArea.X
                + std::max(0, std::min(
                    workArea.Width - appWindow.Size().Width,
                    (int)pointerPosition.x - workArea.X
                )),

            .Y = workArea.Y
                + std::max(0, std::min(
                    workArea.Height - appWindow.Size().Height,
                    (int)pointerPosition.y - workArea.Y
                ))
        };

        appWindow.Move(appWindowPos);
        propertiesWindow.Activate();
    }
}
