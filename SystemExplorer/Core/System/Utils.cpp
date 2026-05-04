#include "pch.h"
#include "Utils.h"
#include <shobjidl.h>    
#include <appmodel.h>   


namespace winrt::SystemExplorer::Core::System::Utils
{
    bool RestartProcess(HANDLE process)
    {
        if (!is_valid(process)) return FALSE;

        std::wstring aumid; 
        std::wstring path; 

        bool isUwp = !aumid.empty();

        TerminateProcess(process, 0);
        WaitForSingleObject(process, INFINITE);

        if (isUwp)
        {
            com_ptr<IApplicationActivationManager> applicationActivationManager;
            if (SUCCEEDED(CoCreateInstance(
                CLSID_ApplicationActivationManager,
                nullptr,
                CLSCTX_LOCAL_SERVER,
                IID_PPV_ARGS(&applicationActivationManager)
            )))
            {
                DWORD newPid = 0;
                return SUCCEEDED(applicationActivationManager->ActivateApplication(aumid.c_str(), nullptr, AO_NONE, &newPid));
            }
            return FALSE;
        }
        else if (!path.empty())
        {
            STARTUPINFOW si = { sizeof(si) };
            wil::unique_process_information pi;

            if (CreateProcessW(path.c_str(), nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi))
            {
                return TRUE;
            }
        }
        return FALSE;
    }

    std::wstring ExtractProcessName(PSYSTEM_PROCESS_INFORMATION pInfo, IN uint32_t pid)
    {
        if (pInfo->ImageName.Buffer != nullptr)
            return { pInfo->ImageName.Buffer, pInfo->ImageName.Length / sizeof(WCHAR) };
        if (pid == 0) return L"Idle";
        if (pid == 4) return L"System";
        return L"Unknown";
    }

    bool IsProcessSuspended(PSYSTEM_PROCESS_INFORMATION pInfo)
    {
        if (pInfo->NumberOfThreads == 0) return false;

        for (ULONG i = 0; i < pInfo->NumberOfThreads; i++)
        {
            if (pInfo->Threads[i].ThreadState != KTHREAD_STATE::Standby || pInfo->Threads[i].WaitReason != Suspended)
                return false;
        }
        return true;
    }
}