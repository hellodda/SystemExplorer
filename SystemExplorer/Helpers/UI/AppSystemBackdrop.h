#pragma once
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Composition.SystemBackdrops.h>

#include "Helpers/UI/AppSystemBackdrop.g.h"

#include <Core/Data/EventArguments/SettingChangedEventArgs.h>
#include <winrt/SystemExplorer.Core.Data.Enums.h>
#include <factory.h>

namespace winrt::SystemExplorer::Helpers::UI::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Media;

    using namespace winrt::Microsoft::UI::Composition;
    using namespace winrt::Microsoft::UI::Composition::SystemBackdrops;

    using namespace winrt::SystemExplorer::Core::Data::EventArguments;
    using namespace winrt::SystemExplorer::Core::Data::Enums;

    struct AppSystemBackdrop : AppSystemBackdropT<AppSystemBackdrop>
    {
        AppSystemBackdrop();

        AppSystemBackdrop(bool active);

        void OnTargetConnected(
            winrt::Microsoft::UI::Composition::ICompositionSupportsSystemBackdrop const& connectedTarget,
            winrt::Microsoft::UI::Xaml::XamlRoot const& xamlRoot
        );
        void OnTargetDisconnected(
            winrt::Microsoft::UI::Composition::ICompositionSupportsSystemBackdrop const& connectedTarget
        );
        void OnDefaultSystemBackdropConfigurationChanged(
            winrt::Microsoft::UI::Composition::ICompositionSupportsSystemBackdrop const& target,
            winrt::Microsoft::UI::Xaml::XamlRoot const& xamlRoot
        );

    private:
        void onSettingChanged(IInspectable const& sender, SettingChangedEventArgs const& args);
        void setThinAcrylicBackdropProperties(DesktopAcrylicController const& controller, SystemBackdropTheme theme);

        ISystemBackdropControllerWithTargets getSystemBackdropController(BackdropMaterialType backdropType, SystemBackdropTheme theme);
    private:
        ISystemBackdropControllerWithTargets controller_{ nullptr };
        ICompositionSupportsSystemBackdrop target_{ nullptr };
        SystemBackdropTheme prevTheme_;
        XamlRoot root_{ nullptr };

        bool isSecondaryWindow_{ false };
    };
}
FACTORY(winrt::SystemExplorer::Helpers::UI, AppSystemBackdrop);