#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOMCX
#define NOHELP
#define NOCOMM
#include <mimalloc-override.h>
#include <unknwn.h>
#include <windows.h>

#pragma warning(push)
#pragma warning(disable: 4201)
#pragma warning(disable: 4471) 
#pragma warning(disable: 4324) 
#pragma warning(disable: 4005) 

#include <phnt_windows.h>
#include <phnt.h>

#pragma warning(pop)
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

#include <ppl.h>

#include <winrt/base.h>
#include <wil/win32_helpers.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>


//#include <ExplorerCore.h>