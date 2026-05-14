#pragma once
#include <amsi.h>
#include <vector>
#include "../../../../Common/wilx.h"

namespace winrt::SystemExplorer::Core::System::Tools
{
    enum class AmsiScanState
    {
        Clean,             
        NotDetected,      
        BlockedByAdmin,    
        Malware            
    };

    using unique_amsi_context = wilx::unique_any<&::AmsiUninitialize>;

    struct AmsiScanner final
    {
        explicit AmsiScanner(hstring const& appName);

        ~AmsiScanner();

        [[nodiscard]] AmsiScanState ScanBuffer(PCVOID buffer, uint32_t length, hstring const& contentName);
        [[nodiscard]] AmsiScanState ScanBuffer(std::vector<uint8_t> const& buffer, hstring const& contentName);
    private:
        static unique_amsi_context context_;
        HAMSISESSION session_{ nullptr };
    private:
        static AmsiScanState parseAmsiResult(AMSI_RESULT result);
    };
}