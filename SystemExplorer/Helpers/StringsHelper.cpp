#include "pch.h"
#include "StringsHelper.h"

#if __has_include("Helpers/StringsHelper.g.cpp")
#include "Helpers/StringsHelper.g.cpp"
#endif

namespace winrt::SystemExplorer::Helpers::implementation
{
    winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader StringsHelper::loader_{};

    hstring StringsHelper::Acrylic()
    {
        return loader_.GetString(L"Acrylic");
    }

    hstring StringsHelper::Blue()
    {
        return loader_.GetString(L"Blue");
    }

    hstring StringsHelper::BlueGray()
    {
        return loader_.GetString(L"BlueGray");
    }

    hstring StringsHelper::BrickRed()
    {
        return loader_.GetString(L"BrickRed");
    }

    hstring StringsHelper::Camouflage()
    {
        return loader_.GetString(L"Camouflage");
    }

    hstring StringsHelper::CoolBlueBright()
    {
        return loader_.GetString(L"CoolBlueBright");
    }

    hstring StringsHelper::Custom()
    {
        return loader_.GetString(L"Custom");
    }

    hstring StringsHelper::Dark()
    {
        return loader_.GetString(L"Dark");
    }

    hstring StringsHelper::Default()
    {
        return loader_.GetString(L"Default");
    }

    hstring StringsHelper::Gray()
    {
        return loader_.GetString(L"Gray");
    }

    hstring StringsHelper::GrayDark()
    {
        return loader_.GetString(L"GrayDark");
    }

    hstring StringsHelper::Green()
    {
        return loader_.GetString(L"Green");
    }

    hstring StringsHelper::IrisPastel()
    {
        return loader_.GetString(L"IrisPastel");
    }

    hstring StringsHelper::Light()
    {
        return loader_.GetString(L"Light");
    }

    hstring StringsHelper::Mica()
    {
        return loader_.GetString(L"Mica");
    }

    hstring StringsHelper::MicaAlt()
    {
        return loader_.GetString(L"MicaAlt");
    }

    hstring StringsHelper::MintLight()
    {
        return loader_.GetString(L"MintLight");
    }

    hstring StringsHelper::ModRed()
    {
        return loader_.GetString(L"ModRed");
    }

    hstring StringsHelper::NavRootHomeContent()
    {
        return loader_.GetString(L"Nav.Root.Home.Content");
    }

    hstring StringsHelper::NavRootMonitoring()
    {
        return loader_.GetString(L"Nav.Root.Monitoring");
    }

    hstring StringsHelper::NavRootSystem()
    {
        return loader_.GetString(L"Nav.Root.System");
    }

    hstring StringsHelper::OrangeBright()
    {
        return loader_.GetString(L"OrangeBright");
    }

    hstring StringsHelper::Overcast()
    {
        return loader_.GetString(L"Overcast");
    }

    hstring StringsHelper::Red()
    {
        return loader_.GetString(L"Red");
    }

    hstring StringsHelper::RoseBright()
    {
        return loader_.GetString(L"RoseBright");
    }

    hstring StringsHelper::Seafoam()
    {
        return loader_.GetString(L"Seafoam");
    }

    hstring StringsHelper::Static()
    {
        return loader_.GetString(L"Static");
    }

    hstring StringsHelper::Storm()
    {
        return loader_.GetString(L"Storm");
    }

    hstring StringsHelper::SystemTheme()
    {
        return loader_.GetString(L"SystemTheme");
    }

    hstring StringsHelper::ThinAcrylic()
    {
        return loader_.GetString(L"ThinAcrylic");
    }

    hstring StringsHelper::VioletRedLight()
    {
        return loader_.GetString(L"VioletRedLight");
    }

    hstring StringsHelper::YellowGold()
    {
        return loader_.GetString(L"YellowGold");
    }
}
