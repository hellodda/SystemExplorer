#pragma once

#include "Helpers/UI/AppSystemBackdrop.g.h"

#include <winrt/SystemExplorer.Core.Data.Enums.h>

#include <factory.h>

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::UI::Xaml::Interop;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Media;
    using namespace winrt::Microsoft::UI::Composition;
    using namespace winrt::Microsoft::UI::Composition::SystemBackdrops;
}

namespace winrt::SystemExplorer::Helpers::UI::implementation
{
    struct AppSystemBackdrop : AppSystemBackdropT<AppSystemBackdrop>
    {
        AppSystemBackdrop();

        AppSystemBackdrop(bool active);

        static inline wil::single_threaded_property<DependencyProperty> EnableWhenInactiveProperty = DependencyProperty::Register(
            L"EnableWhenInactive",
            xaml_typename<bool>(),
            xaml_typename<class_type>(),
            nullptr
        );

        void EnableWhenInactive(bool value) noexcept;
        [[nodiscard]] bool EnableWhenInactive() const noexcept;

        void OnTargetConnected(
            winrt::ICompositionSupportsSystemBackdrop const& connectedTarget,
            winrt::XamlRoot const& xamlRoot
        );
        void OnTargetDisconnected(
            winrt::ICompositionSupportsSystemBackdrop const& connectedTarget
        );
        void OnDefaultSystemBackdropConfigurationChanged(
            winrt::ICompositionSupportsSystemBackdrop const& target,
            winrt::XamlRoot const& xamlRoot
        );

    private:
        void onSettingChanged();
        void setThinAcrylicBackdropProperties(DesktopAcrylicController const& controller, winrt::SystemBackdropTheme theme);

        [[nodiscard]] ISystemBackdropControllerWithTargets getSystemBackdropController(Core::Data::Enums::BackdropMaterialType backdropType, winrt::SystemBackdropTheme theme);
    private:
        ISystemBackdropControllerWithTargets controller_{ nullptr };
        ICompositionSupportsSystemBackdrop target_{ nullptr };
        SystemBackdropTheme prevTheme_;
        XamlRoot root_{ nullptr };
    };
}
FACTORY(winrt::SystemExplorer::Helpers::UI, AppSystemBackdrop);