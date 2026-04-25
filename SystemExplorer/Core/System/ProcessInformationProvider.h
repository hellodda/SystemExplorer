#pragma once
#include "System.h"
#include "pthread.h"
#include <unordered_map>
#include <wil/resource.h> 

#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "version.lib")

namespace winrt::SystemExplorer::Core::System
{
    using namespace winrt::SystemExplorer::Core::System::Contracts;

    struct ProcessCacheEntry
    {
        LARGE_INTEGER CreateTime{};
        ULONGLONG LastSystemTime{};
        ULONGLONG LastProcessTime{};
        ULONGLONG LastIoTransferCount{};
        uint64_t LastTickCount{};

        hstring Name;
        hstring Description;
    };

    struct ProcessInformationProvider : public IProcessInformationProvider
    {
        ProcessInformationProvider();

        std::vector<ProcessInformation> GetAllProcesses() override;

        IProviderThread* Thread() override;

    private:
        void updateProcesses();
    private:
        std::unique_ptr<IProviderThread> thread_;

        wil::srwlock lock_;
        std::vector<ProcessInformation> activeProcesses_;
        std::unordered_map<uint32_t, ProcessCacheEntry> processCache_;

        ULONG bufferSize_;
        wil::unique_virtualalloc_ptr<BYTE> buffer_;
    };
}