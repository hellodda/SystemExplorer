#include "pch.h"
#include "StringsHelper.h"

#if __has_include("Helpers/StringsHelper.g.cpp")
#include "Helpers/StringsHelper.g.cpp"
#endif

namespace winrt::SystemExplorer::Helpers::implementation
{
    hstring StringsHelper::Acrylic()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Acrylic");
    }

    hstring StringsHelper::Blue()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Blue");
    }

    hstring StringsHelper::BlueGray()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"BlueGray");
    }

    hstring StringsHelper::BrickRed()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"BrickRed");
    }

    hstring StringsHelper::Camouflage()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Camouflage");
    }

    hstring StringsHelper::CoolBlueBright()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"CoolBlueBright");
    }

    hstring StringsHelper::Custom()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Custom");
    }

    hstring StringsHelper::Default()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Default");
    }

    hstring StringsHelper::Gray()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Gray");
    }

    hstring StringsHelper::GrayDark()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"GrayDark");
    }

    hstring StringsHelper::Green()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Green");
    }

    hstring StringsHelper::IrisPastel()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"IrisPastel");
    }

    hstring StringsHelper::Mica()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Mica");
    }

    hstring StringsHelper::MicaAlt()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"MicaAlt");
    }

    hstring StringsHelper::MintLight()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"MintLight");
    }

    hstring StringsHelper::ModRed()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"ModRed");
    }

    hstring StringsHelper::NavRootHomeContent()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Nav.Root.Home.Content");
    }

    hstring StringsHelper::NavRootMonitoring()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Nav.Root.Monitoring");
    }

    hstring StringsHelper::NavRootSystem()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Nav.Root.System");
    }

    hstring StringsHelper::OrangeBright()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"OrangeBright");
    }

    hstring StringsHelper::Overcast()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Overcast");
    }

    hstring StringsHelper::Red()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Red");
    }

    hstring StringsHelper::RoseBright()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"RoseBright");
    }

    hstring StringsHelper::Seafoam()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Seafoam");
    }

    hstring StringsHelper::Static()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Static");
    }

    hstring StringsHelper::Storm()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Storm");
    }

    hstring StringsHelper::ThinAcrylic()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"ThinAcrylic");
    }

    hstring StringsHelper::VioletRedLight()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"VioletRedLight");
    }

    hstring StringsHelper::YellowGold()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"YellowGold");
    }

    hstring StringsHelper::DarkText()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Dark.Text");
    }

    hstring StringsHelper::LightText()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Light.Text");
    }

    hstring StringsHelper::SystemThemeText()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"SystemTheme.Text");
    }

    hstring StringsHelper::None()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"None");
    }

    hstring StringsHelper::Fill()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Fill");
    }

    hstring StringsHelper::Uniform()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Uniform");
    }

    hstring StringsHelper::UniformToFill()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"UniformToFill");
    }

    hstring StringsHelper::Top()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Top");
    }

    hstring StringsHelper::Center()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Center");
    }

    hstring StringsHelper::Bottom()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Bottom");
    }

    hstring StringsHelper::Left()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Left");
    }

    hstring StringsHelper::Right()
    {
        return winrt::Microsoft::Windows::ApplicationModel::Resources::ResourceLoader().GetString(L"Right");
    }
}
