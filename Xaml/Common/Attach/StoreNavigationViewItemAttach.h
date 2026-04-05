#pragma once

#include "Common/StoreNavigationViewItemAttach.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::SystemExplorer::Xaml::Common::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;

    struct StoreNavigationViewItemAttach : StoreNavigationViewItemAttachT<StoreNavigationViewItemAttach>
    {
        StoreNavigationViewItemAttach() = default;

        static inline const wil::single_threaded_property<DependencyProperty> SelectedIconProperty = DependencyProperty::RegisterAttached(
            L"SelectedIcon",
            xaml_typename<IInspectable>(),
            xaml_typename<class_type>(),
            nullptr
        );

        static inline const wil::single_threaded_property<DependencyProperty> ShowNotificationDotProperty = DependencyProperty::RegisterAttached(
            L"ShowNotificationDot",
            xaml_typename<bool>(),
            xaml_typename<class_type>(),
            nullptr
        );

        static inline const wil::single_threaded_property<DependencyProperty> UnselectedIconProperty = DependencyProperty::RegisterAttached(
            L"UnselectedIcon",
            xaml_typename<IInspectable>(),
            xaml_typename<class_type>(),
            nullptr
        );

        static inline const wil::single_threaded_property<DependencyProperty> StaticIconVisibilityProperty = DependencyProperty::RegisterAttached(
            L"StaticIconVisibility",
            xaml_typename<Visibility>(),
            xaml_typename<class_type>(),
            nullptr
        );

        static IInspectable GetSelectedIcon(DependencyObject const& obj) noexcept;
        static void SetSelectedIcon(DependencyObject const& obj, IInspectable const& value);

        static IInspectable GetUnselectedIcon(DependencyObject const& obj) noexcept;
        static void SetUnselectedIcon(DependencyObject const& obj, IInspectable const& value);

        static bool GetShowNotificationDot(DependencyObject const& obj) noexcept;
        static void SetShowNotificationDot(DependencyObject const& obj, bool value);

        static Visibility GetStaticIconVisibility(DependencyObject const& obj) noexcept;
        static void SetStaticIconVisibility(DependencyObject const& obj, Visibility const& value);
    };
}

namespace winrt::SystemExplorer::Xaml::Common::factory_implementation
{
    struct StoreNavigationViewItemAttach : StoreNavigationViewItemAttachT<StoreNavigationViewItemAttach, implementation::StoreNavigationViewItemAttach>
    {};
}
