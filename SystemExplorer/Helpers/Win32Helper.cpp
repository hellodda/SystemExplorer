#include "pch.h"
#include "Win32Helper.h"

namespace winrt::SystemExplorer::Helpers
{
    std::wstring Win32Helper::GetErrorMessage(DWORD code)
    {
        wil::unique_hmodule ntdll(GetModuleHandleW(L"ntdll.dll"));
        LPWSTR messageBuffer{ nullptr };

        auto size = FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            (ntdll ? FORMAT_MESSAGE_FROM_HMODULE : 0) | FORMAT_MESSAGE_IGNORE_INSERTS,
            ntdll.get(),
            code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&messageBuffer,
        0, NULL);

        if (size > 0 && messageBuffer) {
            wil::unique_process_heap_ptr<wchar_t> smartBuffer(messageBuffer);
            return std::wstring(smartBuffer.get());
        }
        return L"Unknown Error";
    }
    std::wstring Win32Helper::GetLocalizedResource(UINT id)
    {
        static auto module{ GetModuleHandle(NULL) };
        LPWSTR buffer{ nullptr };

        int len = LoadStringW(module, id, reinterpret_cast<LPWSTR>(&buffer), 0);

        if (len > 0 && buffer)
        {
            return { buffer, static_cast<size_t>(len) };
        }
        return L"";
    }
    std::wstring Win32Helper::GetCurrentProcessPath()
    {
        WCHAR path[MAX_PATH];
        GetModuleFileNameW(NULL, path, MAX_PATH);

        return std::wstring(L"\"") + path + L"\"";
    }
}

