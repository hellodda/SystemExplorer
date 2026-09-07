#pragma once
#ifndef SYSX_EMS_BASE_H
#define SYSX_EMS_BASE_H

#define SYSX_API_EXPORTS

#ifdef SYSX_STATIC_API
#define SYSX_EMS_API
#elif defined(SYSX_API_EXPORTS)
#define SYSX_EMS_API __declspec(dllexport)
#else
#define SYSX_EMS_API __declspec(dllimport)
#endif

#define SYSX_TRANSPORT_TYPE_PIPES
#define SYSX_TRANSPORT_TYPE_ALPC

#undef SYSX_TRANSPORT_TYPE_ALPC

#define SYSX_ALPC_ENDPOINT L""
#define SYSX_PIPE_ENDPOINT L"\\\\.\\pipe\\sysx.extensions"

#define SYSX_DEFAULT_TIMEOUT 2500

#include <windows.h>

typedef HRESULT EMS_STATUS;

#endif // !SYSX_EMS_BASE_H
