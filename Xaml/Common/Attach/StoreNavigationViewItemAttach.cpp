#include "pch.h"
#include "StoreNavigationViewItemAttach.h"
#if __has_include("Common.StoreNavigationViewItemAttach.g.cpp")
#include "Common.StoreNavigationViewItemAttach.g.cpp"
#endif

namespace winrt::SystemExplorer::Xaml::Common::implementation
{
    IInspectable StoreNavigationViewItemAttach::GetSelectedIcon(DependencyObject const& obj) noexcept
    {
        return obj.GetValue(SelectedIconProperty());
    }

    void StoreNavigationViewItemAttach::SetSelectedIcon(DependencyObject const& obj, IInspectable const& value)
    {
        obj.SetValue(SelectedIconProperty(), value);
    }

    // UnselectedIcon
    IInspectable StoreNavigationViewItemAttach::GetUnselectedIcon(DependencyObject const& obj) noexcept
    {
        return obj.GetValue(UnselectedIconProperty());
    }

    void StoreNavigationViewItemAttach::SetUnselectedIcon(DependencyObject const& obj, IInspectable const& value)
    {
        obj.SetValue(UnselectedIconProperty(), value);
    }

    // ShowNotificationDot
    bool StoreNavigationViewItemAttach::GetShowNotificationDot(DependencyObject const& obj) noexcept
    {
        return unbox_value<bool>(obj.GetValue(ShowNotificationDotProperty()));
    }

    void StoreNavigationViewItemAttach::SetShowNotificationDot(DependencyObject const& obj, bool value)
    {
        obj.SetValue(ShowNotificationDotProperty(), box_value(value));
    }

    // StaticIconVisibility
    Visibility StoreNavigationViewItemAttach::GetStaticIconVisibility(DependencyObject const& obj) noexcept
    {
        return unbox_value<Visibility>(obj.GetValue(StaticIconVisibilityProperty()));
    }

    void StoreNavigationViewItemAttach::SetStaticIconVisibility(DependencyObject const& obj, Visibility const& value)
    {
        obj.SetValue(StaticIconVisibilityProperty(), box_value(value));
    }
}
