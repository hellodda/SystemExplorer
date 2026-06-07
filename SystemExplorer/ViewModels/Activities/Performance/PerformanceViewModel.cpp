#include "pch.h"
#include "PerformanceViewModel.h"
#if __has_include("ViewModels/Activities/Performance/PerformanceViewModel.g.cpp")
#include "ViewModels/Activities/Performance/PerformanceViewModel.g.cpp"
#endif
#include <Views/Pages/Activities/Performance/CpuPerformancePage.xaml.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

namespace winrt::SystemExplorer::ViewModels::Activities::Performance::implementation
{
	PerformanceViewModel::PerformanceViewModel()
	{


        PerformanceNavigationViewItem cpuItem;
        cpuItem.Header(L"CPU");
        cpuItem.Description(L"4% 1.65 GHz");
        cpuItem.PageName(xaml_typename<Views::Pages::Activities::Performance::CpuPerformancePage>().Name);
        NavigationViewItems.Append(cpuItem);

        // 2. Memory
        PerformanceNavigationViewItem memItem;
        memItem.Header(L"Memory");
        memItem.Description(L"10.3/15.6 GB");
        memItem.SubDescription(L"(66%)");
        NavigationViewItems.Append(memItem);

        // 3. Disk
        PerformanceNavigationViewItem diskItem;
        diskItem.Header(L"Disk 0 (C:)");
        diskItem.Description(L"SSD (NVMe)");
        diskItem.SubDescription(L"1%");
        NavigationViewItems.Append(diskItem);

        // 4. Ethernet
        PerformanceNavigationViewItem ethItem;
        ethItem.Header(L"Ethernet");
        ethItem.Description(L"Ethernet 2");
        ethItem.SubDescription(L"S: 0 R: 0 Kbps");
        NavigationViewItems.Append(ethItem);

        // 5. Wi-Fi
        PerformanceNavigationViewItem wifiItem;
        wifiItem.Header(L"Wi-Fi");
        wifiItem.Description(L"Wi-Fi");
        wifiItem.SubDescription(L"S: 0 R: 0 Kbps");
        NavigationViewItems.Append(wifiItem);

        // 6. GPU
        PerformanceNavigationViewItem gpuItem;
        gpuItem.Header(L"GPU 0");
        gpuItem.Description(L"Intel(R) Iris(R) Xe Graphics");
        gpuItem.SubDescription(L"2%");
        NavigationViewItems.Append(gpuItem);
	}
}
