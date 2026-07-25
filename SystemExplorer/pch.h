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

#undef GetCurrentTime
#include <winrt/WinUI3Package.h>
#include <ppl.h>
#include <pplawait.h>
#include <pplcancellation_token.h>
#include <filesystem>
#include <ExplorerCore.h>