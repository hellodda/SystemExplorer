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

        static winrt::Microsoft::UI::Xaml::DependencyProperty SelectedIconProperty();
        static winrt::Microsoft::UI::Xaml::DependencyProperty ShowNotificationDotProperty();
        static winrt::Microsoft::UI::Xaml::DependencyProperty UnselectedIconProperty();
        static winrt::Microsoft::UI::Xaml::DependencyProperty StaticIconVisibilityProperty();

        // 获取和设置附加属性的方法
        static winrt::Windows::Foundation::IInspectable GetSelectedIcon(winrt::Microsoft::UI::Xaml::DependencyObject const& obj);
        static void SetSelectedIcon(winrt::Microsoft::UI::Xaml::DependencyObject const& obj, winrt::Windows::Foundation::IInspectable const& value);
        static bool GetShowNotificationDot(winrt::Microsoft::UI::Xaml::DependencyObject const& obj);
        static void SetShowNotificationDot(winrt::Microsoft::UI::Xaml::DependencyObject const& obj, bool value);
        static winrt::Windows::Foundation::IInspectable GetUnselectedIcon(winrt::Microsoft::UI::Xaml::DependencyObject const& obj);
        static void SetUnselectedIcon(winrt::Microsoft::UI::Xaml::DependencyObject const& obj, winrt::Windows::Foundation::IInspectable const& value);
        static winrt::Microsoft::UI::Xaml::Visibility GetStaticIconVisibility(winrt::Microsoft::UI::Xaml::DependencyObject const& obj);
        static void SetStaticIconVisibility(winrt::Microsoft::UI::Xaml::DependencyObject const& obj, winrt::Microsoft::UI::Xaml::Visibility const& value);

    private:
        static winrt::Microsoft::UI::Xaml::DependencyProperty s_selectedIconProperty;
        static winrt::Microsoft::UI::Xaml::DependencyProperty s_showNotificationDotProperty;
        static winrt::Microsoft::UI::Xaml::DependencyProperty s_unselectedIconProperty;
        static winrt::Microsoft::UI::Xaml::DependencyProperty s_staticIconVisibilityProperty;
    };
}

namespace winrt::SystemExplorer::Xaml::Common::factory_implementation
{
    struct StoreNavigationViewItemAttach : StoreNavigationViewItemAttachT<StoreNavigationViewItemAttach, implementation::StoreNavigationViewItemAttach>
    {};
}
