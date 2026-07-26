#pragma once

#include "Views/Pages/Activities/Performance/CpuPerformancePage.g.h"

namespace winrt
{
    using namespace winrt::Microsoft::UI::Xaml;
}

namespace winrt::SystemExplorer::Views::Pages::Activities::Performance::implementation
{
    struct CpuPerformancePage : CpuPerformancePageT<CpuPerformancePage>
    {
        CpuPerformancePage();
    private:
        //std::unique_ptr<PerformanceGraph> graph_{ nullptr };
        //winrt::DispatcherTimer timer_{};
    };
}
FACTORY(winrt::SystemExplorer::Views::Pages::Activities::Performance, CpuPerformancePage);