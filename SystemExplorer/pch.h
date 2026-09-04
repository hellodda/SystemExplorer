#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOMCX
#define NOHELP
#define NOCOMM
#include <mimalloc-override.h>
#include <unknwn.h>
#include <windows.h>

#define __ImageBase __ImageBase_phnt_ignore
#include <phnt_windows.h>
#include <phnt.h>
#undef __ImageBase
#pragma comment(lib, "ntdll.lib")
#include <wtsapi32.h>
#pragma comment(lib, "wtsapi32.lib")

#include <restrictederrorinfo.h>
#include <ShlObj.h>

#undef GetCurrentTime
#include <ppl.h>
#include <pplawait.h>
#include <wil/stl.h>
#include <factory.h>
#include <property.h>
#include <wil/result.h>
#include <wil/win32_helpers.h>
#include <pplcancellation_token.h>
#include <filesystem>
#include <ranges>
#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <memory>
#include <cassert>
#include <functional>
#include <map>
#include <mutex>
#include <regex>

#undef min
#undef max
#include <rapidfuzz/fuzz.hpp>


#include <ppl.h>

#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>

// override basic winrt functions
#include "winrt_override.h"

#include <gsl/gsl>
#include <wil/win32_helpers.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
//#include <wil/stl.h>
//#include <wil/result.h>
//#include <wil/win32_helpers.h>
//#include <wil/cppwinrt.h>
//#include <wil/cppwinrt_helpers.h>
//#include <wil/wistd_type_traits.h>
//#include <wil/cppwinrt_authoring.h>
//#include <wil/resource.h>

#include <winrt/SystemExplorer.Xaml.h>
#include <winrt/SystemExplorer.h>
#include <winrt/XamlToolkit.WinUI.h>
#include <winrt/XamlToolkit.Labs.WinUI.h>
#include <winrt/XamlToolkit.WinUI.Controls.h>
#include <winrt/XamlToolkit.WinUI.Media.h>
#include <winrt/XamlToolkit.WinUI.Helpers.h>
#include <winrt/XamlToolkit.WinUI.Rive.h>
#include <winrt/XamlToolkit.WinUI.Converters.h>
#include <winrt/WinUI3Package.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Metadata.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.UI.h>

#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Globalization.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.ApplicationModel.Activation.h>

#include <winrt/Microsoft.UI.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Text.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <winrt/Microsoft.UI.Xaml.XamlTypeInfo.h>
#include <winrt/Microsoft.UI.Composition.SystemBackdrops.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Microsoft.Windows.Storage.Pickers.h>
#include <winrt/Microsoft.Windows.Storage.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.Windows.ApplicationModel.Resources.h>

#include <winrt/XamlToolkit.WinUI.h>
#include <winrt/XamlToolkit.WinUI.Converters.h>
#include <winrt/XamlToolkit.WinUI.Helpers.h>
#include <winrt/XamlToolkit.WinUI.Controls.h>
#include <winrt/XamlToolkit.WinUI.Animations.h>

#include <winrt/SystemExplorer.Core.Data.Enums.h>
#include <winrt/SystemExplorer.Helpers.h>

#include <wil/cppwinrt.h>
#include <wil/cppwinrt_helpers.h>

#include "Xaml/Controls/SettingsGroup/SimpleSettingsGroup/SimpleSettingsGroup.h"
#include "Xaml/Controls/SettingsGroup/SimpleSettingsGroup/SimpleSettingsGroupAutomationPeer.h"
#include "Xaml/Mvvm/AsyncRelayCommand.h"
#include "Xaml/Mvvm/AsyncRelayCommandFactory.h"
#include "Xaml/Mvvm/RelayCommand.h"
#include "Xaml/Selectors/ItemTemplateSelector.h"

