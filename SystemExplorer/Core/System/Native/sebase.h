/*
 * Copyright (c) 2026 SystemExplorer, All rights reserved.
 *
 * This file is part of System Explorer.
 *
 * Authors:
 *
 *     hellodda 2026
 *
 */

#pragma once

#ifndef _SE_PHBASE_H
#define _SE_PHBASE_H

#ifndef SELIB_NO_DEFAULT_LIB
#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "winsta.lib")
#endif


#ifdef __clang__
#define SE_CLANG_STRINGIFYX(x)         #x
#define SE_CLANG_STRINGIFY(x)          SE_CLANG_STRINGIFYX(x)
#define SE_CLANG_DIAGNOSTIC_PUSH()     _Pragma("clang diagnostic push")
#define SE_CLANG_DIAGNOSTIC_IGNORED(x) _Pragma(SE_CLANG_STRINGIFY(clang diagnostic ignored x))
#define SE_CLANG_DIAGNOSTIC_POP()      _Pragma("clang diagnostic pop")
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
#define typeof __typeof__
#endif
#else
#define SE_CLANG_DIAGNOSTIC_PUSH()
#define SE_CLANG_DIAGNOSTIC_IGNORED(x)
#define SE_CLANG_DIAGNOSTIC_POP()
#endif

#include <windows.h>
#include <phnt_windows.h>
#include <phnt_ntdef.h>
#include <phnt.h>

#endif