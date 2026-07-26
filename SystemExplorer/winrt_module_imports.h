#pragma once

#ifdef __INTELLISENSE__
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
#include <winrt/Microsoft.UI.Xaml.Animation.h>
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
#include <winrt/Microsoft.UI.Xaml.>
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
#else
#define WINRT_IMPORT_MODULE

import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Collections;
import winrt.Windows.Foundation.Metadata;
import winrt.Windows.Foundation.Numerics;
import winrt.Windows.UI;
import winrt.Windows.UI.Xaml.Interop;
import winrt.Windows.UI.Notifications;
import winrt.Windows.Graphics.Imaging;
import winrt.Windows.Globalization;
import winrt.Windows.Graphics.Imaging;
import winrt.Windows.System;
import winrt.Windows.Storage;
import winrt.Windows.Storage.Pickers;
import winrt.Windows.Storage.Streams;
import winrt.Windows.Data.Xml.Dom;

import winrt.Microsoft.UI;
import winrt.Microsoft.UI.Composition;
import winrt.Microsoft.UI.Windowing;
import winrt.Microsoft.UI.Text;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Data;
import winrt.Microsoft.UI.Xaml.Interop;
import winrt.Microsoft.UI.Xaml.Media;
import winrt.Microsoft.UI.Xaml.Media.Imaging;
import winrt.Microsoft.UI.Xaml.Media.Animation;
import winrt.Microsoft.UI.Xaml.Controls;
import winrt.Microsoft.UI.Xaml.Hosting;
import winrt.Microsoft.UI.Xaml.Markup;
import winrt.Microsoft.UI.Xaml.Shapes;
import winrt.Microsoft.UI.Dispatching;
import winrt.Microsoft.UI.Xaml.XamlTypeInfo;
import winrt.Microsoft.UI.Composition.SystemBackdrops;
import winrt.Microsoft.Windows.Storage.Pickers;
import winrt.Microsoft.Windows.Storage;
import winrt.Microsoft.Windows.ApplicationModel.Resources;
import winrt.Microsoft.Windows.AppLifecycle;


import winrt.XamlToolkit.WinUI;
import winrt.XamlToolkit.Labs.WinUI;
import winrt.XamlToolkit.WinUI.Media;
import winrt.XamlToolkit.WinUI.Converters;
import winrt.XamlToolkit.WinUI.Helpers;
import winrt.XamlToolkit.WinUI.Controls;
import winrt.XamlToolkit.WinUI.Animations;
import winrt.XamlToolkit.WinUI.Rive;

import winrt.WinUI3Package;

import winrt.SystemExplorer.Xaml;
import winrt.SystemExplorer.Converters;
import winrt.SystemExplorer.Views.Pages;
import winrt.SystemExplorer.Views.Pages.Settings;
import winrt.SystemExplorer.Views.Pages.Activities;
import winrt.SystemExplorer.Views.Pages.Activities.Performance;
import winrt.SystemExplorer.Views.Windows;
import winrt.SystemExplorer.ViewModels;
import winrt.SystemExplorer.ViewModels.Settings;
import winrt.SystemExplorer.ViewModels.Activities;
import winrt.SystemExplorer.ViewModels.Activities.Performance;
import winrt.SystemExplorer.Helpers;
import winrt.SystemExplorer.Core.Data.Enums;
import winrt.SystemExplorer.Core.Data.Items;
import winrt.SystemExplorer.Core.Data.Models;

#include <wil_cppwinrt_module.h>
#endif
