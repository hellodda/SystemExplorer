#include "pch.h"
#include "Win32Helper.h"

typedef struct LANGANDCODEPAGE {
    WORD wLanguage;
    WORD wCodePage;
} *PLANGANDCODEPAGE, FAR* LPLANGANDCODEPAGE;

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
    uint64_t Win32Helper::GetCurrentSystemTime()
    {
        FILETIME idleTime{}, kernelTime{}, userTime{};

        if (!GetSystemTimes(&idleTime, &kernelTime, &userTime))
            return 0;

        return FileTimeToInteger(kernelTime) + FileTimeToInteger(userTime);
    }
    uint64_t Win32Helper::FileTimeToInteger(FILETIME const& fileTime)
    {
        return (static_cast<uint64_t>(fileTime.dwHighDateTime) << 32) | fileTime.dwLowDateTime;
    }
    std::wstring Win32Helper::GetFileVersionString(HANDLE handle, std::wstring_view key)
    {
        if (!handle || handle == INVALID_HANDLE_VALUE)
            return {};

        WCHAR fullPath[MAX_PATH]{};
        DWORD sizePath = MAX_PATH;

        if (!QueryFullProcessImageNameW(handle, 0, fullPath, &sizePath))
            return {};

        DWORD dummy = 0;
        DWORD size = GetFileVersionInfoSizeW(fullPath, &dummy);
        if (!size)
            return {};

        std::vector<BYTE> versionData(size);

        if (!GetFileVersionInfoW(fullPath, 0, size, versionData.data()))
            return {};

        PLANGANDCODEPAGE translate = nullptr;
        UINT cbTranslate = 0;

        if (!VerQueryValueW(versionData.data(),
            L"\\VarFileInfo\\Translation",
            (LPVOID*)&translate,
            &cbTranslate))
            return {};

        if (cbTranslate < sizeof(LANGANDCODEPAGE))
            return {};

        WCHAR subBlock[128]{};
        swprintf_s(subBlock, L"\\StringFileInfo\\%04x%04x\\%s",
            translate[0].wLanguage,
            translate[0].wCodePage,
            key.data());

        LPVOID buffer = nullptr;
        UINT bytes = 0;

        if (VerQueryValueW(versionData.data(), subBlock, &buffer, &bytes))
        {
            return std::wstring(static_cast<wchar_t*>(buffer));
        }

        return {};
    }
    std::wstring Win32Helper::ProcessHelper::GetProcessDescription(uint32_t pid)
    {
        if (pid == 0 || pid == 4) return {};

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
        if (!hProcess) return {};

        wil::unique_handle processHandle(hProcess);
        
        return Win32Helper::GetFileVersionString(processHandle.get(), L"FileDescription");
    }
}

