#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOMCX
#define NOHELP
#define NOCOMM

#include <windows.h>
#include <phnt_windows.h>
#include <phnt.h>
#include <restrictederrorinfo.h>
#include <ShlObj.h>

#undef GetCurrentTime
#include <ppl.h>
#include <pplawait.h>
#include <wil/stl.h>
#include <wil/result.h>
#include <wil/win32_helpers.h>
#include <pplcancellation_token.h>
#include <filesystem>
#include <ExplorerCore.h>