#pragma once
// ---------------------------------------------------------------------
// Windows SDK / Platform
// ---------------------------------------------------------------------
#include <windows.h>
#include <commdlg.h>
#include <atlbase.h>
#include <wchar.h>

// ---------------------------------------------------------------------
// WMI (Windows Management Instrumentation)
// ---------------------------------------------------------------------
#include <WbemIdl.h>
#include <comutil.h>
#include <WMIUtils.h>
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "comsuppw.lib")

// ---------------------------------------------------------------------
// NT / Internal headers (PHNT)
// ---------------------------------------------------------------------
#include <phnt_windows.h>
#include <phnt.h>
#pragma comment(lib, "ntdll.lib")

// ---------------------------------------------------------------------
// WinRT / Metadata / COM
// ---------------------------------------------------------------------
#include <cor.h>
#include <corhdr.h>
#include <rometadata.h>
#include <winrt/base.h>
#pragma comment(lib, "rometadata.lib")
#pragma comment(lib, "runtimeobject.lib")

// ---------------------------------------------------------------------
// WIL (Windows Implementation Libraries)
// ---------------------------------------------------------------------
#include <wil/resource.h>
#include <wil/win32_helpers.h>

// ---------------------------------------------------------------------
// PPL (Parallel Patterns Library)
// ---------------------------------------------------------------------
#include <ppl.h>
#include <pplawait.h>
#include <pplcancellation_token.h>

// ---------------------------------------------------------------------
// GSL (Guideline Support Library)
// ---------------------------------------------------------------------
#include <gsl/gsl>

// ---------------------------------------------------------------------
// C++ Standard Library
// ---------------------------------------------------------------------
#include <future>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <optional>

// ---------------------------------------------------------------------
// Link additional Windows libraries
// ---------------------------------------------------------------------
#pragma comment(lib, "Comdlg32.lib")
