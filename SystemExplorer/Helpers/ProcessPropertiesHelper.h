#pragma once

#ifdef __INTELLISENSE__
#include <wil/cppwinrt_authoring.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Windowing.h>
#endif

#include <Views/Pages/Properties/PropertiesRootPage.xaml.h>

namespace winrt::SystemExplorer::Helpers
{
    using namespace winrt::WinUI3Package;
    using namespace winrt::Windows::UI::Xaml::Interop;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::Windows::Foundation;
    using namespace winrt::SystemExplorer::Core::Data::Items;


    struct ProcessPropertiesHelper
    {
        ProcessPropertiesHelper() = default;

        static void OpenPropertiesWindow(IInspectable const& item);
    private:

        static void PropertiesWindows_Closed(IInspectable const& sender, WindowEventArgs const& args)
        {

        }
    };
}