#pragma once
#include "Helpers/StringsHelper.g.h"
#include <winrt/Microsoft.Windows.ApplicationModel.Resources.h>

namespace winrt::SystemExplorer::Helpers::implementation
{
    using namespace winrt::Microsoft::Windows::ApplicationModel::Resources;

    struct StringsHelper : StringsHelperT<StringsHelper>
    {
       [[nodiscard]] static hstring Acrylic();
       [[nodiscard]] static hstring Blue();
       [[nodiscard]] static hstring BlueGray();
       [[nodiscard]] static hstring BrickRed();
       [[nodiscard]] static hstring Camouflage();
       [[nodiscard]] static hstring CoolBlueBright();
       [[nodiscard]] static hstring Custom();
       [[nodiscard]] static hstring Default();
       [[nodiscard]] static hstring Gray();
       [[nodiscard]] static hstring GrayDark();
       [[nodiscard]] static hstring Green();
       [[nodiscard]] static hstring IrisPastel();
       [[nodiscard]] static hstring Mica();
       [[nodiscard]] static hstring MicaAlt();
       [[nodiscard]] static hstring MintLight();
       [[nodiscard]] static hstring ModRed();
       [[nodiscard]] static hstring NavRootHomeContent();
       [[nodiscard]] static hstring NavRootMonitoring();
       [[nodiscard]] static hstring NavRootSystem();
       [[nodiscard]] static hstring OrangeBright();
       [[nodiscard]] static hstring Overcast();
       [[nodiscard]] static hstring Red();
       [[nodiscard]] static hstring RoseBright();
       [[nodiscard]] static hstring Seafoam();
       [[nodiscard]] static hstring Static();
       [[nodiscard]] static hstring Storm();
       [[nodiscard]] static hstring ThinAcrylic();
       [[nodiscard]] static hstring VioletRedLight();
       [[nodiscard]] static hstring YellowGold();
       [[nodiscard]] static hstring DarkText();
       [[nodiscard]] static hstring LightText();
       [[nodiscard]] static hstring SystemThemeText();
       [[nodiscard]] static hstring None();
       [[nodiscard]] static hstring Fill();
       [[nodiscard]] static hstring Uniform();
       [[nodiscard]] static hstring UniformToFill();
       [[nodiscard]] static hstring Top();
       [[nodiscard]] static hstring Center();
       [[nodiscard]] static hstring Bottom();
       [[nodiscard]] static hstring Left();
       [[nodiscard]] static hstring Right();

    private:
        StringsHelper() = default;

        static ResourceLoader loader_;
    };
}

namespace winrt::SystemExplorer::Helpers::factory_implementation
{
    struct StringsHelper : StringsHelperT<StringsHelper, implementation::StringsHelper>
    {
    };
}
