#include "pch.h"
#include "StoreNavigationViewItemAttach.h"
#if __has_include("Common/StoreNavigationViewItemAttach.g.cpp")
#include "Common/StoreNavigationViewItemAttach.g.cpp"
#endif


using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace Windows::Foundation;

namespace winrt::SystemExplorer::Xaml::Common::implementation
{
	Microsoft::UI::Xaml::DependencyProperty StoreNavigationViewItemAttach::s_selectedIconProperty =
		Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
			L"SelectedIcon",
			winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
			winrt::xaml_typename<class_type>(),
			Microsoft::UI::Xaml::PropertyMetadata(nullptr));

	Microsoft::UI::Xaml::DependencyProperty StoreNavigationViewItemAttach::s_showNotificationDotProperty =
		Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
			L"ShowNotificationDot",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<class_type>(),
			Microsoft::UI::Xaml::PropertyMetadata(winrt::box_value(false)));

	Microsoft::UI::Xaml::DependencyProperty StoreNavigationViewItemAttach::s_unselectedIconProperty =
		Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
			L"UnselectedIcon",
			winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
			winrt::xaml_typename<class_type>(),
			Microsoft::UI::Xaml::PropertyMetadata(nullptr));

	Microsoft::UI::Xaml::DependencyProperty StoreNavigationViewItemAttach::s_staticIconVisibilityProperty =
		Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
			L"StaticIconVisibility",
			winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Visibility>(),
			winrt::xaml_typename<class_type>(),
			Microsoft::UI::Xaml::PropertyMetadata(winrt::box_value(Microsoft::UI::Xaml::Visibility::Collapsed)));

	/// <summary>
	/// 获取 SelectedIcon 附加属性的依赖属性对象
	/// 用于在导航项选中状态下显示的图标
	/// </summary>
	/// <returns>SelectedIcon 依赖属性的静态实例</returns>
	/// 
	Microsoft::UI::Xaml::DependencyProperty StoreNavigationViewItemAttach::SelectedIconProperty()
	{
		return s_selectedIconProperty;
	}

	/// <summary>
	/// 获取 ShowNotificationDot 附加属性的依赖属性对象
	/// 用于控制是否在导航项上显示通知圆点
	/// </summary>
	/// <returns>ShowNotificationDot 依赖属性的静态实例</returns>
	Microsoft::UI::Xaml::DependencyProperty StoreNavigationViewItemAttach::ShowNotificationDotProperty()
	{
		return s_showNotificationDotProperty;
	}

	/// <summary>
	/// 获取 UnselectedIcon 附加属性的依赖属性对象
	/// 用于在导航项未选中状态下显示的图标
	/// </summary>
	/// <returns>UnselectedIcon 依赖属性的静态实例</returns>
	Microsoft::UI::Xaml::DependencyProperty StoreNavigationViewItemAttach::UnselectedIconProperty()
	{
		return s_unselectedIconProperty;
	}

	/// <summary>
	/// 获取 StaticIconVisibility 附加属性的依赖属性对象
	/// 用于控制静态图标（自定义图标）的可见性
	/// </summary>
	/// <returns>StaticIconVisibility 依赖属性的静态实例</returns>
	Microsoft::UI::Xaml::DependencyProperty StoreNavigationViewItemAttach::StaticIconVisibilityProperty()
	{
		return s_staticIconVisibilityProperty;
	}

	/// <summary>
	/// 获取指定对象的 SelectedIcon 属性值
	/// </summary>
	/// <param name="obj">要获取属性值的依赖对象（通常是 NavigationViewItem）</param>
	/// <returns>选中状态的图标对象</returns>
	IInspectable StoreNavigationViewItemAttach::GetSelectedIcon(Microsoft::UI::Xaml::DependencyObject const& obj)
	{
		// 从依赖对象获取附加属性的值
		return obj.GetValue(SelectedIconProperty());
	}

	/// <summary>
	/// 设置指定对象的 SelectedIcon 属性值
	/// </summary>
	/// <param name="obj">要设置属性值的依赖对象（通常是 NavigationViewItem）</param>
	/// <param name="value">要设置的选中状态图标对象</param>
	void StoreNavigationViewItemAttach::SetSelectedIcon(Microsoft::UI::Xaml::DependencyObject const& obj, IInspectable const& value)
	{
		// 为依赖对象设置附加属性的值
		obj.SetValue(SelectedIconProperty(), value);
	}

	/// <summary>
	/// 获取指定对象的 ShowNotificationDot 属性值
	/// </summary>
	/// <param name="obj">要获取属性值的依赖对象</param>
	/// <returns>是否显示通知圆点的布尔值</returns>
	bool StoreNavigationViewItemAttach::GetShowNotificationDot(Microsoft::UI::Xaml::DependencyObject const& obj)
	{
		// 获取布尔值属性需要使用 unbox_value 进行类型转换
		return unbox_value<bool>(obj.GetValue(ShowNotificationDotProperty()));
	}

	/// <summary>
	/// 设置指定对象的 ShowNotificationDot 属性值
	/// </summary>
	/// <param name="obj">要设置属性值的依赖对象</param>
	/// <param name="value">是否显示通知圆点</param>
	void StoreNavigationViewItemAttach::SetShowNotificationDot(Microsoft::UI::Xaml::DependencyObject const& obj, bool value)
	{
		// 设置布尔值属性需要使用 box_value 进行装箱
		obj.SetValue(ShowNotificationDotProperty(), box_value(value));
	}

	/// <summary>
	/// 获取指定对象的 UnselectedIcon 属性值
	/// </summary>
	/// <param name="obj">要获取属性值的依赖对象</param>
	/// <returns>未选中状态的图标对象</returns>
	IInspectable StoreNavigationViewItemAttach::GetUnselectedIcon(Microsoft::UI::Xaml::DependencyObject const& obj)
	{
		// 从依赖对象获取未选中图标属性的值
		return obj.GetValue(UnselectedIconProperty());
	}

	/// <summary>
	/// 设置指定对象的 UnselectedIcon 属性值
	/// </summary>
	/// <param name="obj">要设置属性值的依赖对象</param>
	/// <param name="value">要设置的未选中状态图标对象</param>
	void StoreNavigationViewItemAttach::SetUnselectedIcon(Microsoft::UI::Xaml::DependencyObject const& obj, IInspectable const& value)
	{
		// 为依赖对象设置未选中图标属性的值
		obj.SetValue(UnselectedIconProperty(), value);
	}

	/// <summary>
	/// 获取指定对象的 StaticIconVisibility 属性值
	/// </summary>
	/// <param name="obj">要获取属性值的依赖对象</param>
	/// <returns>静态图标的可见性状态</returns>
	Microsoft::UI::Xaml::Visibility StoreNavigationViewItemAttach::GetStaticIconVisibility(Microsoft::UI::Xaml::DependencyObject const& obj)
	{
		// 获取可见性枚举值需要使用 unbox_value 进行类型转换
		return unbox_value<Microsoft::UI::Xaml::Visibility>(obj.GetValue(StaticIconVisibilityProperty()));
	}

	/// <summary>
	/// 设置指定对象的 StaticIconVisibility 属性值
	/// </summary>
	/// <param name="obj">要设置属性值的依赖对象</param>
	/// <param name="value">静态图标的可见性状态</param>
	void StoreNavigationViewItemAttach::SetStaticIconVisibility(Microsoft::UI::Xaml::DependencyObject const& obj, Microsoft::UI::Xaml::Visibility const& value)
	{
		// 设置可见性枚举值需要使用 box_value 进行装箱
		obj.SetValue(StaticIconVisibilityProperty(), box_value(value));
	}
}
