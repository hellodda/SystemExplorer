#pragma once
#include "Views/Pages/Settings/SettingsRootPage.g.h"
#include <winrt/Microsoft.UI.Xaml.h>
#include <factory.h>

namespace winrt::SystemExplorer::Views::Pages::Settings::implementation
{
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Windows::Foundation;

    struct SettingsRootPage : SettingsRootPageT<SettingsRootPage>
    {
        SettingsRootPage()
        {
            InitializeComponent();

            auto vector = winrt::single_threaded_vector<winrt::Windows::Foundation::IInspectable>();

            // 2. Упаковываем строки с помощью box_value и добавляем в вектор
            vector.Append(winrt::box_value(L"Main"));
            vector.Append(winrt::box_value(L"Remote access"));

            // Теперь XAML без проблем примет этот список
            SettingsBreadcrumbBar().ItemsSource(vector);
        }
        IAsyncAction SettingsFrameLoaded(IInspectable const& sender, RoutedEventArgs const& args);
        IAsyncAction SettingsNavViewSelectionChanged(NavigationView const& sender, NavigationViewSelectionChangedEventArgs const& args);
    };
}

FACTORY(winrt::SystemExplorer::Views::Pages::Settings, SettingsRootPage);