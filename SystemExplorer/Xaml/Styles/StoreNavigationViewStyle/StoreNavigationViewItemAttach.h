#pragma once

#include "Xaml/StoreNavigationViewItemAttach.g.h"

#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;Xaml/
}

namespace winrt::SystemExplorer::Xaml::implementation
{
    struct StoreNavigationViewItemAttach : StoreNavigationViewItemAttachT<StoreNavigationViewItemAttach>
    {
        StoreNavigationViewItemAttach() = default;

        static inline wil::single_threaded_property<DependencyProperty> SelectedIconProperty = DependencyProperty::RegisterAttached(
            L"SelectedIcon",
            xaml_typename<IInspectable>(),
            xaml_typename<class_type>(),
            PropertyMetadata(nullptr)
        );
        static inline wil::single_threaded_property<DependencyProperty> ShowNotificationDotProperty = DependencyProperty::RegisterAttached(
            L"ShowNotificationDot",
            xaml_typename<bool>(),
            xaml_typename<class_type>(),
            PropertyMetadata(box_value(false))
        );
        static inline wil::single_threaded_property<DependencyProperty> UnselectedIconProperty = DependencyProperty::RegisterAttached(
            L"UnselectedIcon",
            xaml_typename<IInspectable>(),
            xaml_typename<class_type>(),
            PropertyMetadata(nullptr)
        );
        static inline wil::single_threaded_property<DependencyProperty> StaticIconVisibilityProperty = DependencyProperty::RegisterAttached(
            L"StaticIconVisibility",
            xaml_typename<Visibility>(),
            xaml_typename<class_type>(),
            PropertyMetadata(box_value(Visibility::Collapsed))
        );

        [[nodiscard]] static IInspectable GetSelectedIcon(DependencyObject const& obj);
        static void SetSelectedIcon(DependencyObject const& obj, IInspectable const& value);
        [[nodiscard]] static bool GetShowNotificationDot(DependencyObject const& obj);
        static void SetShowNotificationDot(DependencyObject const& obj, bool value);
        [[nodiscard]] static IInspectable GetUnselectedIcon(DependencyObject const& obj);
        static void SetUnselectedIcon(DependencyObject const& obj, IInspectable const& value);
        [[nodiscard]] static Visibility GetStaticIconVisibility(DependencyObject const& obj);
        static void SetStaticIconVisibility(DependencyObject const& obj, Visibility const& value);
    };
}

namespace winrt::SystemExplorer::Xaml::factory_implementation
{
    struct StoreNavigationViewItemAttach : StoreNavigationViewItemAttachT<StoreNavigationViewItemAttach, implementation::StoreNavigationViewItemAttach>
    {};
}
