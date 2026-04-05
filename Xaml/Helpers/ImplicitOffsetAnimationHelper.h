#pragma once

#include "Helpers/ImplicitOffsetAnimationHelper.g.h"

#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::SystemExplorer::Xaml::Helpers::implementation
{
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Controls;

    struct ImplicitOffsetAnimationHelper : ImplicitOffsetAnimationHelperT<ImplicitOffsetAnimationHelper>
    {
        ImplicitOffsetAnimationHelper() = default;

        static void OnEnableOffsetAnimationChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static inline const wil::single_threaded_property<DependencyProperty> EnableOffsetAnimationProperty = DependencyProperty::RegisterAttached(
            L"EnableOffsetAnimation",
            xaml_typename<bool>(),
            xaml_typename<class_type>(),
            PropertyMetadata(nullptr, &ImplicitOffsetAnimationHelper::OnEnableOffsetAnimationChanged)
        );

        [[nodiscard]] static bool GetEnableOffsetAnimation(UIElement const& element) noexcept { return unbox_value<bool>(element.GetValue(EnableOffsetAnimationProperty)); }
        static void SetEnableOffsetAnimation(UIElement const& element, bool value) { element.SetValue(EnableOffsetAnimationProperty, box_value(value)); }
    };
}

namespace winrt::SystemExplorer::Xaml::Helpers::factory_implementation
{
    struct ImplicitOffsetAnimationHelper : ImplicitOffsetAnimationHelperT<ImplicitOffsetAnimationHelper, implementation::ImplicitOffsetAnimationHelper>
    {
    };
}
