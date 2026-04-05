#include "pch.h"
#include "VisualAttach.h"
#if __has_include("Common/VisualAttach.g.cpp")
#include "Common/VisualAttach.g.cpp"
#endif
#include <winrt/Windows.Foundation.Numerics.h>

using namespace winrt::Microsoft::UI::Composition;
using namespace winrt::Microsoft::UI::Xaml::Hosting;
using namespace winrt::Windows::Foundation::Numerics;

namespace winrt::SystemExplorer::Xaml::Common::implementation
{
	inline float2 ParseStringToFloat2(winrt::hstring const& str)
	{
		float x{ 0.0f };
		float y{ 0.0f };

		auto s = winrt::to_string(str);
		size_t comma = s.find(',');
		if (comma != std::string::npos)
		{
			x = std::stof(s.substr(0, comma));
			y = std::stof(s.substr(comma + 1));
		}
		return { x, y };
	}
	inline float3 ParseStringToFloat3(winrt::hstring const& str)
	{
		float x = 0.0f, y = 0.0f, z = 0.0f;
		auto s = winrt::to_string(str);
		size_t firstComma = s.find(',');
		size_t secondComma = s.rfind(',');
		if (firstComma != std::string::npos && secondComma != std::string::npos && firstComma != secondComma)
		{
			x = std::stof(s.substr(0, firstComma));
			y = std::stof(s.substr(firstComma + 1, secondComma - firstComma - 1));
			z = std::stof(s.substr(secondComma + 1));
		}
		return { x, y, z };
	}
	inline winrt::hstring Float2ToString(float2 const& v)
	{
		std::wostringstream ss;
		ss << v.x << "," << v.y;
		return winrt::hstring(ss.str());
	}
	inline winrt::hstring Float3ToString(float3 const& v)
	{
		std::wostringstream ss;
		ss << v.x << "," << v.y << "," << v.z;
		return winrt::hstring(ss.str());
	}


	void VisualAttach::OnNormalizedCenterPointChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto element = d.try_as<FrameworkElement>())
		{
			if (auto newValue = e.NewValue().try_as<hstring>())
			{
				auto center = ParseStringToFloat2(newValue.value());
				auto visual = ElementCompositionPreview::GetElementVisual(element);

				static constexpr auto expression = L"Vector2(this.Target.Size.X * X, this.Target.Size.Y * Y)";

				auto animation = visual.Compositor().CreateExpressionAnimation(expression);
				animation.SetScalarParameter(L"X", center.x);
				animation.SetScalarParameter(L"Y", center.y);

				visual.StopAnimation(L"CenterPoint.XY");
				visual.StartAnimation(L"CenterPoint.XY", animation);
			}
		}
	}
	void VisualAttach::OnScaleChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto string = e.NewValue().try_as<hstring>())
		{
			SetScale(d, string.value_or(L"1"));
		}
	}
	bool VisualAttach::GetIsBindCenterPoint(DependencyObject const& obj) noexcept
	{
		return unbox_value<bool>(obj.GetValue(IsBindCenterPointProperty));
	}
	void VisualAttach::SetIsBindCenterPoint(DependencyObject const& obj, bool value)
	{
		obj.SetValue(IsBindCenterPointProperty, box_value(value));
	}
	hstring VisualAttach::GetNormalizedCenterPoint(DependencyObject const& obj) noexcept
	{
		return unbox_value<hstring>(obj.GetValue(NormalizedCenterPointProperty));
	}
	void VisualAttach::SetNormalizedCenterPoint(DependencyObject const& obj, hstring const& value)
	{
		obj.SetValue(NormalizedCenterPointProperty, box_value(value));
	}
	hstring VisualAttach::GetScale(DependencyObject const& obj) noexcept
	{
		if (auto element = obj.try_as<UIElement>())
		{
			return GetScaleForElement(element);
		}
		return unbox_value<hstring>(obj.GetValue(ScaleProperty));
	}
	void VisualAttach::SetScale(DependencyObject const& obj, hstring const& value)
	{
		if (auto element = obj.try_as<UIElement>())
		{
			SetScaleForElement(value, element);
		}
		obj.SetValue(ScaleProperty, box_value(value));
	}
	hstring VisualAttach::GetScaleForElement(UIElement const& element) noexcept
	{
		auto visual = ElementCompositionPreview::GetElementChildVisual(element);
		return Float3ToString(visual.Scale());
	}
	void VisualAttach::SetScaleForElement(hstring const& value, UIElement const& element)
	{
		auto visual = ElementCompositionPreview::GetElementVisual(element);
		visual.Scale(ParseStringToFloat3(value));
	}
}
