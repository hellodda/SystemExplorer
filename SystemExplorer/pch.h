#pragma once

#include <windows.h>
#include <phnt_windows.h>
#include <phnt.h>
#include <unknwn.h>
#include <restrictederrorinfo.h>
#include <hstring.h>
#include <winrt/Windows.Foundation.h>

#pragma region winrt extensions

namespace winrt 
{
    inline hstring to_hstring(Windows::Foundation::IInspectable const& value)
    {
        if (!value) return L"null";
        if (auto stringable = value.try_as<Windows::Foundation::IStringable>()) {
            return stringable.ToString();
        }
        return L"Object (Unknown type)";
    }
}

#pragma endregion

#undef GetCurrentTime
#include <winrt/SystemExplorer.h>
#include <winrt/XamlToolkit.WinUI.h>
#include <winrt/XamlToolkit.Labs.WinUI.h>
#include <winrt/XamlToolkit.WinUI.Controls.h>
#include <winrt/XamlToolkit.WinUI.Media.h>
#include <winrt/XamlToolkit.WinUI.Helpers.h>
#include <winrt/XamlToolkit.WinUI.Rive.h>
#include <winrt/XamlToolkit.WinUI.Converters.h>
#include <winrt/WinUI3Package.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <wil/stl.h>
#include <wil/cppwinrt_helpers.h>
#include <wil/result.h>
#include <wil/win32_helpers.h>
#include <wil/cppwinrt_authoring.h>
#include <winrt/SystemExplorer.Xaml.h>
#include <ppl.h>
#include <pplawait.h>
#include <pplcancellation_token.h>
#include <filesystem>