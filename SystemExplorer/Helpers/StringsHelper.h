#pragma once
#include "Helpers/StringsHelper.g.h"
#include <factory.h>
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
        [[nodiscard]] static hstring Performance();
        [[nodiscard]] static hstring System();
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
        [[nodiscard]] static hstring Dark();
        [[nodiscard]] static hstring Light();
        [[nodiscard]] static hstring SystemTheme();
        [[nodiscard]] static hstring None();
        [[nodiscard]] static hstring Fill();
        [[nodiscard]] static hstring Uniform();
        [[nodiscard]] static hstring UniformToFill();
        [[nodiscard]] static hstring Top();
        [[nodiscard]] static hstring Center();
        [[nodiscard]] static hstring Bottom();
        [[nodiscard]] static hstring Left();
        [[nodiscard]] static hstring Right();
        [[nodiscard]] static hstring SystemSetting();
        [[nodiscard]] static hstring Language();
        [[nodiscard]] static hstring Yes();
        [[nodiscard]] static hstring No();
        [[nodiscard]] static hstring UpdateSpeed();
        [[nodiscard]] static hstring High();
        [[nodiscard]] static hstring Low();
        [[nodiscard]] static hstring Suspended();
        [[nodiscard]] static hstring DefaultStartPage();
        [[nodiscard]] static hstring Processes();
        [[nodiscard]] static hstring Services();
        [[nodiscard]] static hstring WindowManagement();
        [[nodiscard]] static hstring DymamicKernelMemoryDump();
        [[nodiscard]] static hstring RestoreSettings();
        [[nodiscard]] static hstring ResetSettings();
        [[nodiscard]] static hstring Additionally();
        [[nodiscard]] static hstring General();
        [[nodiscard]] static hstring Appearance();
        [[nodiscard]] static hstring Backdrop();
        [[nodiscard]] static hstring ApplicationTheme();
        [[nodiscard]] static hstring BackgroundColor();
        [[nodiscard]] static hstring Review();
        [[nodiscard]] static hstring Remove();
        [[nodiscard]] static hstring Opacity();
        [[nodiscard]] static hstring ImageFit();
        [[nodiscard]] static hstring VerticalAlignment();
        [[nodiscard]] static hstring HorizontalAlignment();
        [[nodiscard]] static hstring Advanced();
        [[nodiscard]] static hstring ExportSettings();
        [[nodiscard]] static hstring ImportSettings();
        [[nodiscard]] static hstring EditSettingsFile();
        [[nodiscard]] static hstring ExperimentalFunctions();
        [[nodiscard]] static hstring UseDriver();
        [[nodiscard]] static hstring AboutApplication();
        [[nodiscard]] static hstring SupportOnGitHub();
        [[nodiscard]] static hstring HelpAndSupport();
        [[nodiscard]] static hstring Documentation();
        [[nodiscard]] static hstring OpenSource();
        [[nodiscard]] static hstring RestartNotificationText();
        [[nodiscard]] static hstring Activities();
        [[nodiscard]] static hstring About();
        [[nodiscard]] static hstring ImageFiles();
        [[nodiscard]] static hstring BitmapFiles();

    private:
        StringsHelper() = default;
    };
}
FACTORY(winrt::SystemExplorer::Helpers, StringsHelper)
