#include "pch.h"
#include "ServicesPage.xaml.h"
#if __has_include("Views/Pages/Activities/ServicesPage.g.cpp")
#include "Views/Pages/Activities/ServicesPage.g.cpp"
#endif

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::SystemExplorer::Views::Pages::Activities::implementation
{
    ServicesPage::ServicesPage()
    {
        InitializeComponent();
    }
}