#include "pch.h"
#include "Win32Helper.h"
#include <shobjidl.h>   
#include <appmodel.h> 

#pragma comment(lib, "Shell32.lib")

#ifndef SHGFI_LIMITICON
#define SHGFI_LIMITICON 0x00010000
#endif

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

    [[nodiscard]] uint64_t Win32Helper::FileTimeToInteger(FILETIME const& fileTime)
    {
        return std::bit_cast<uint64_t>(fileTime);
    }

    std::wstring Win32Helper::GetFileVersionString(HANDLE handle, std::wstring_view key)
    {
        if (!handle || handle == INVALID_HANDLE_VALUE)
            return {};

        wchar_t path[MAX_PATH];
        DWORD pathSize = MAX_PATH;
        if (!QueryFullProcessImageNameW(handle, 0, path, &pathSize)) return {};

        DWORD dummy;
        DWORD size = GetFileVersionInfoSizeW(path, &dummy);
        if (size == 0) return {};

        auto data = std::make_unique_for_overwrite<std::byte[]>(size);
        if (!GetFileVersionInfoW(path, 0, size, data.get())) return {};

        PLANGANDCODEPAGE translate;
        UINT length;

        if (!VerQueryValueW(data.get(), L"\\VarFileInfo\\Translation", (LPVOID*)&translate, &length) || length < sizeof(LANGANDCODEPAGE))
            return {};

        auto subBlock = std::format(L"\\StringFileInfo\\{:04x}{:04x}\\{}",
            translate->wLanguage, translate->wCodePage, key);

        wchar_t* buffer;
        if (VerQueryValueW(data.get(), subBlock.c_str(), (LPVOID*)&buffer, &length))
            return buffer;

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

    std::wstring Win32Helper::ProcessHelper::GetProcessDescription(HANDLE process)
    {
        return Win32Helper::GetFileVersionString(process, L"FileDescription");
    }

    std::wstring Win32Helper::ProcessHelper::GetProcessAMUID(uint32_t id)
    {
        return std::wstring();
    }

    std::wstring Win32Helper::ProcessHelper::GetProcessAMUID(HANDLE process)
    {
        UINT32 length{ 0 };
        LONG rc = GetApplicationUserModelId(process, &length, NULL);

        if (rc != ERROR_INSUFFICIENT_BUFFER)
            return {};

        std::wstring amuid;

        rc = GetApplicationUserModelId(process, &length, amuid.data());
    }

    wil::unique_hicon Win32Helper::ProcessHelper::GetProcessIcon(uint32_t id)
    {
        wil::unique_process_handle process{ OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, id) };
        
        return GetProcessIcon(process.get());
    }

    wil::unique_hicon Win32Helper::ProcessHelper::GetProcessIcon(HANDLE process)
    {
        wil::unique_hicon icon;
        auto path = GetProcessPath(process);

        if (path.size() > 0)
        {
            ExtractIconExW(path.c_str(), NULL, nullptr, icon.put(), 1);
        }
        if (!icon.is_valid())
        {
            icon.reset(LoadIcon(NULL, IDI_APPLICATION));
        }
        return icon;
    }

    std::wstring Win32Helper::ProcessHelper::GetProcessPath(HANDLE process)
    {
        if (!process || process == INVALID_HANDLE_VALUE) return {};

        DWORD size{ MAX_PATH };
        std::wstring buffer(size, L'\0');

        while (true)
        {
            if (QueryFullProcessImageNameW(process, 0, buffer.data(), &size))
            {
                buffer.resize(size);
                return buffer;
            }

            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                size *= 2;
                buffer.resize(size);
            }
            else 
                return {};
        }
    }
    bool Win32Helper::ProcessHelper::IsProcessEfficiencyModeEnabled(uint32_t id)
    {
        wil::unique_process_handle process{ OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, id) };
        return IsProcessEfficiencyModeEnabled(process.get());
    }
    bool Win32Helper::ProcessHelper::IsProcessEfficiencyModeEnabled(HANDLE process)
    {
        if (process != INVALID_HANDLE_VALUE) return false;

        PROCESS_POWER_THROTTLING_STATE state{};
        state.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;

        if (GetProcessInformation(process, ProcessPowerThrottling, &state, sizeof(state)))
        {
            return (state.ControlMask & PROCESS_POWER_THROTTLING_EXECUTION_SPEED) &&
                (state.StateMask & PROCESS_POWER_THROTTLING_EXECUTION_SPEED);
        }
        return false;
    }
}