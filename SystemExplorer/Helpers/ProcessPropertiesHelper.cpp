#include "pch.h"
#include "ProcessPropertiesHelper.h"
#if __has_include("Helpers/ProcessPropertiesHelper.g.cpp")
#include "Helpers/ProcessPropertiesHelper.g.cpp"
#include "StringsHelper.h"
#endif
#include <Core/Data/Parameters/PropertiesPageNavigationParameter.h>
#include <Helpers/UI/AppSystemBackdrop.h>
#include <App.xaml.h>

#undef max
#undef min

using namespace winrt::Windows::Graphics;
using namespace winrt::Microsoft::UI::Windowing;
using namespace winrt::SystemExplorer::Core::Data::Parameters;
using namespace winrt::WinUI3Package;

namespace winrt::SystemExplorer::Helpers::implementation
{
	void ProcessPropertiesHelper::OpenPropertiesWindow(ISystemItem const& item)
	{
        if (!item)
            return;

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

        //appWindow.SetIcon();

        frame.Navigate(
            xaml_typename<SystemExplorer::Views::Pages::Properties::PropertiesRootPage>(),
            PropertiesPageNavigationParameter
            {
                item
            }
        );

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
