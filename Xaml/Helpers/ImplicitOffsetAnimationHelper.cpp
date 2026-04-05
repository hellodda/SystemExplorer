#include "pch.h"
#include "ImplicitOffsetAnimationHelper.h"
#if __has_include("Helpers/ImplicitOffsetAnimationHelper.g.cpp")
#include "Helpers/ImplicitOffsetAnimationHelper.g.cpp"
#endif
#include <winrt/Microsoft.UI.Xaml.Hosting.h>

namespace winrt::SystemExplorer::Xaml::Helpers::implementation
{
    void ImplicitOffsetAnimationHelper::OnEnableOffsetAnimationChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
    {
        if (auto element = d.try_as<FrameworkElement>())
        {
            if (auto newValue = e.NewValue().try_as<bool>(); newValue && *newValue)
            {

                element.Loaded([element = element](auto&&, auto&&)
                {
                    auto visual = Hosting::ElementCompositionPreview::GetElementVisual(element);
                    auto compositor = visual.Compositor();

                    auto animation = compositor.CreateVector3KeyFrameAnimation();
                    animation.Target(L"Offset");
                    animation.Duration(std::chrono::milliseconds(400));
                    animation.InsertExpressionKeyFrame(1.0f, L"this.FinalValue");

                    auto implicitAnimations = compositor.CreateImplicitAnimationCollection();
                    implicitAnimations.Insert(L"Offset", animation);

                    visual.ImplicitAnimations(implicitAnimations);
                });
            }
        }
    }
}
