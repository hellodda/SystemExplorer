#include "pch.h"
#include "winrt_module_imports.h"
#include "CpuPerformancePage.xaml.h"
#if __has_include("Views/Pages/Activities/Performance/CpuPerformancePage.g.cpp")
#include "Views/Pages/Activities/Performance/CpuPerformancePage.g.cpp"
#endif
#include  <Helpers/UI/DesktopNotification.h>


namespace winrt::SystemExplorer::Views::Pages::Activities::Performance::implementation
{
	CpuPerformancePage::CpuPerformancePage()
	{
        InitializeComponent();

        //GraphSettings gsettings;
        //gsettings.BgColor = D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f);
        //gsettings.GridColor = D2D1::ColorF(D2D1::ColorF::White, 0.50f);
        //gsettings.GridCellWidth = 70;
        //gsettings.GridCellHeight = 55;
        //graph_ = std::make_unique<PerformanceGraph>(gsettings);

        //graph_->Initialize(GraphSwapChainPanel());

        //LineSettings settings;
        //settings.Name = L"CPU";
        //settings.LineThickness = 1.0f;
        //settings.ShowFill = true;
        //settings.FillColor = D2D1::ColorF(D2D1::ColorF::Orange, 0.25f);
        //settings.LineColor = D2D1::ColorF(D2D1::ColorF::Orange);
        //size_t cpuLineIndex = graph_->AddLine(settings);

        //LineSettings ksettings;
        //ksettings.Name = L"Kernel";
        //ksettings.Style = GraphLineStyle::Dashed;
        //ksettings.LineThickness = 1.0f;
        //ksettings.ShowFill = true;
        //ksettings.LineColor = D2D1::ColorF(D2D1::ColorF::Red);
        //ksettings.FillColor = D2D1::ColorF(D2D1::ColorF::Red, 0.15f);
        //size_t kernelIndex = graph_->AddLine(ksettings);
       
        //ThresholdLineSettings lsettings;
        //lsettings.Name = L"Danger limit";
        //lsettings.LineColor = D2D1::ColorF(D2D1::ColorF::Red);
        //lsettings.Style = GraphLineStyle::Dashed;
        //lsettings.Value = 0.6;

        //using namespace winrt::SystemExplorer::Helpers::UI;

     
        //graph_->AddThresholdLine(lsettings);


        //float currentValue = 0.5f;
       
        //timer_.Interval(std::chrono::milliseconds(25));
        //timer_.Tick([this, cpuLineIndex, kernelIndex, currentValue](const auto&, const auto&) mutable
        //{
        //    static std::mt19937 generator(std::random_device{}());

        //    std::uniform_real_distribution<float> delta(-0.02f, 0.02f);

        //    currentValue += delta(generator);
        //    currentValue = std::clamp(currentValue, 0.0f, 1.0f);

        //    graph_->AddPoint(cpuLineIndex, currentValue);
        //    graph_->AddPoint(kernelIndex, currentValue - 0.1f);
        //});
        //timer_.Start();
	}
}
