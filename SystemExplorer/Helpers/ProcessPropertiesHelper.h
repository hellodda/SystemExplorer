#pragma once

#include "Helpers/ProcessPropertiesHelper.g.h"

#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

#include <winrt/Microsoft.UI.Windowing.h>
#include <Views/Pages/Properties/PropertiesRootPage.xaml.h>

#include <factory.h>

namespace winrt::SystemExplorer::Helpers::implementation
{
    using namespace winrt::WinUI3Package;
    using namespace winrt::Windows::UI::Xaml::Interop;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Windows::Foundation;
    using namespace winrt::SystemExplorer::Core::Data::Items;


    struct ProcessPropertiesHelper : ProcessPropertiesHelperT<ProcessPropertiesHelper>
    {
        ProcessPropertiesHelper() = default;

        static void OpenPropertiesWindow(IInspectable const& item);
    private:

        static void PropertiesWindows_Closed(IInspectable const& sender, WindowEventArgs const& args)
        {

        }
    };
}
FACTORY(winrt::SystemExplorer::Helpers, ProcessPropertiesHelper);