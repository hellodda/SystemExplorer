#pragma once

#include "Common/VisualAttach.g.h"

#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.Composition.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::SystemExplorer::Xaml::Common::implementation
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Composition;
    using namespace winrt::Microsoft::UI::Xaml::Hosting;

    struct VisualAttach : VisualAttachT<VisualAttach>
    {
        VisualAttach() = default;

        static void OnNormalizedCenterPointChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);
        static void OnScaleChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static inline const wil::single_threaded_property<DependencyProperty> IsBindCenterPointProperty = DependencyProperty::RegisterAttached(
            L"IsBindCenterPoint",
            xaml_typename<bool>(),
            xaml_typename<class_type>(),
            PropertyMetadata(box_value(false), [](DependencyObject const& s, DependencyPropertyChangedEventArgs const& a)
                {
                    if (a.NewValue() != a.OldValue())
                    {
                        if (auto element = s.try_as<UIElement>())
                        {
                            auto visual = ElementCompositionPreview::GetElementVisual(element);
                            if (unbox_value<bool>(a.NewValue()))
                            {
                                static constexpr auto expression = L"Vector3(this.Target.Size.X / 2, this.Target.Size.Y / 2, 0f)";
                                auto animation = visual.Compositor().CreateExpressionAnimation(expression);
                                visual.StartAnimation(L"CenterPoint", animation);
                            }
                            else
                            {
                                visual.StopAnimation(L"CenterPoint");
                            }
                        }
                    }
                })
        );

        static inline const wil::single_threaded_property<DependencyProperty> NormalizedCenterPointProperty = DependencyProperty::RegisterAttached(
            L"NormalizedCenterPoint",
            xaml_typename<hstring>(),
            xaml_typename<class_type>(),
            PropertyMetadata(nullptr, &VisualAttach::OnNormalizedCenterPointChanged)
        );

        static inline const wil::single_threaded_property<DependencyProperty> ScaleProperty = DependencyProperty::RegisterAttached(
            L"Scale",
            xaml_typename<hstring>(),
            xaml_typename<class_type>(),
            PropertyMetadata(nullptr, &VisualAttach::OnScaleChanged)
        );

        static bool GetIsBindCenterPoint(DependencyObject const& obj) noexcept;
        static void SetIsBindCenterPoint(DependencyObject const& obj, bool value);

        static hstring GetNormalizedCenterPoint(DependencyObject const& obj) noexcept;
        static void SetNormalizedCenterPoint(DependencyObject const& obj, hstring const& value);

        static hstring GetScale(DependencyObject const& obj) noexcept;
        static void SetScale(DependencyObject const& obj, hstring const& value);

        static hstring GetScaleForElement(UIElement const& element) noexcept;
        static void SetScaleForElement(hstring const& value, UIElement const& element);
    };
}

namespace winrt::SystemExplorer::Xaml::Common::factory_implementation
{
    struct VisualAttach : VisualAttachT<VisualAttach, implementation::VisualAttach>
    {};
}
