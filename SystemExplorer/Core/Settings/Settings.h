#pragma once

#ifdef __INTELLISENSE__
#include <wil/cppwinrt_authoring.h>
#include <winrt/SystemExplorer.Core.Data.Enums.h>
#endif

namespace eil
{
	namespace details
	{
		template <size_t N>
		struct fixed_wstring
		{
			wchar_t buf[N]{};
			consteval fixed_wstring(const wchar_t(&str)[N])
			{
				std::copy_n(str, N, buf);
			}

			constexpr operator std::wstring_view() const noexcept {
				return std::wstring_view(buf, N - 1);
			}
		};

		struct settings_property_context
		{
			static inline thread_local settings_property_context* current_context{ nullptr };

			virtual ~settings_property_context() = default;

			virtual winrt::Windows::Foundation::IInspectable get(std::wstring_view key) const = 0;
			virtual void set(std::wstring_view, winrt::Windows::Foundation::IInspectable const& value) = 0;
		};
	}


	template<typename T>
	concept enum_t = std::is_enum_v<T>;

	template<typename T>
	concept setting_t = std::same_as<T, bool>

		|| std::same_as<T, std::int16_t>
		|| std::same_as<T, std::int32_t>
		|| std::same_as<T, std::int64_t>

		|| std::same_as<T, std::uint8_t>
		|| std::same_as<T, std::uint16_t>
		|| std::same_as<T, std::uint32_t>
		|| std::same_as<T, std::uint64_t>

		|| std::same_as<T, float>
		|| std::same_as<T, double>

		|| std::same_as<T, char16_t>

		|| std::same_as<T, winrt::guid>
		|| std::same_as<T, winrt::hstring>

		|| std::same_as<T, winrt::Windows::Foundation::DateTime>
		|| std::same_as<T, winrt::Windows::Foundation::TimeSpan>

		|| std::same_as<T, winrt::Windows::Foundation::Point>
		|| std::same_as<T, winrt::Windows::Foundation::Size>
		|| std::same_as<T, winrt::Windows::Foundation::Rect>
		|| std::is_enum_v<std::remove_cvref_t<T>>

		|| std::same_as<
		T,
		winrt::Windows::Storage::ApplicationDataCompositeValue>;


	template<setting_t T, details::fixed_wstring key>
	struct single_threaded_setting_base
	{
		single_threaded_setting_base(T defaultValue = T{})
			: value_(defaultValue) {}

		operator T () const
		{
			if (!context_) return value_;

			return winrt::unbox_value_or(context_->get(key), value_);
		}

		template<typename Q>
		auto operator=(Q&& q)
		{
			if (context_)
			{
				auto value = winrt::box_value(static_cast<T>(std::forward<Q>(q)));
				context_->set(key, value);
			}
		}

		T value_;
		details::settings_property_context* context_;
	};

	template<setting_t T, details::fixed_wstring key>
	struct single_threaded_setting : single_threaded_setting_base<T, key>
	{
		using base_type = single_threaded_setting_base<T, key>;

		single_threaded_setting(T defaultValue = T{})
			: base_type(defaultValue) {}

		operator T() const
		{
			return static_cast<const base_type&>(*this).operator T();
		}

		T operator()() const
		{
			return *this;
		}

		template <typename Q>
		auto& operator=(Q&& q)
		{
			static_cast<base_type&>(*this) = std::forward<Q>(q);
			return *this;
		}
	};

	template<setting_t T, details::fixed_wstring key>
	struct single_threaded_rw_setting : single_threaded_setting<T, key>
	{
		using base_type = single_threaded_setting<T, key>;

		single_threaded_rw_setting(T defaultValue)
			: base_type(defaultValue) {}

		using base_type::operator();

		template <typename Q>
		auto& operator()(Q&& q)
		{
			return *this = std::forward<Q>(q);
		}

		template <typename Q>
		auto& operator=(Q&& q)
		{
			base_type::operator=(std::forward<Q>(q));
			return *this;
		}
	};

	struct standard_settings_configuration
	{

	};

	
	struct settings_base : details::settings_property_context
	{
		virtual winrt::Windows::Foundation::IInspectable get(std::wstring_view key) const override
		{
			return winrt::Windows::Foundation::IInspectable();
		}

		virtual void set(std::wstring_view, winrt::Windows::Foundation::IInspectable const& value) override
		{

		}
	};
}

namespace winrt
{
    using namespace winrt::Microsoft::UI::Xaml;
}

namespace winrt::SystemExplorer::Core::Settings
{
    struct AdvancedSettings : eil::settings_base
    {
        eil::single_threaded_rw_setting<bool, L"AdvancedAiAgent"> AdvancedAiAgent{ false };
        eil::single_threaded_rw_setting<bool, L"ProcessScaningModel"> ProcessScaningModel{ false };
        eil::single_threaded_rw_setting<bool, L"InStartup"> InStartup{ false };
    };

    struct AppearanceSettings : eil::settings_base
    {
        eil::single_threaded_rw_setting<Data::Enums::BackdropMaterialType, L"BackdropMaterial"> BackdropMaterial{ Data::Enums::BackdropMaterialType::Mica };
        eil::single_threaded_rw_setting<Data::Enums::Stretch, L"AppThemeBackgroundImageFit"> AppThemeBackgroundImageFit{ Data::Enums::Stretch::None };
        eil::single_threaded_rw_setting<winrt::hstring, L"ApplicationBackgroundColor"> ApplicationBackgroundColor{ L"#00000000" };
        eil::single_threaded_rw_setting<winrt::ElementTheme, L"ApplicationTheme"> ApplicationTheme{ winrt::ElementTheme::Light };
        eil::single_threaded_rw_setting<Data::Enums::VerticalAlignment, L"AppThemeBackgroundImageVerticalAlignment"> AppThemeBackgroundImageVerticalAlignment{ Data::Enums::VerticalAlignment::Center };
        eil::single_threaded_rw_setting<Data::Enums::HorizontalAlignment, L"AppThemeBackgroundImageHorizontalAlignment"> AppThemeBackgroundImageHorizontalAlignment{ Data::Enums::HorizontalAlignment::Center };
        eil::single_threaded_rw_setting<winrt::hstring, L"AppThemeBackgroundImageSource"> AppThemeBackgroundImageSource{ L"" };
        eil::single_threaded_rw_setting<float, L"AppThemeBackgroundImageOpacity"> AppThemeBackgroundImageOpacity{ 0.5f };
        eil::single_threaded_rw_setting<int32_t, L"ItemFontSize"> ItemFontSize{ 13 };
        eil::single_threaded_rw_setting<int32_t, L"ItemIconSize"> ItemIconSize{ 18 };
    };

    struct GeneralSettings : eil::settings_base
    {
        eil::single_threaded_rw_setting<uint16_t, L"RealTimeUpdateSpeedMs"> RealTimeUpdateSpeedMs{ 1500 };
        eil::single_threaded_rw_setting<winrt::hstring, L"StartPage"> StartPage{ L"ProcessesPage" };
        eil::single_threaded_rw_setting<bool, L"MemoryDumpCaptureHypervisorPages"> MemoryDumpCaptureHypervisorPages{ false };
        eil::single_threaded_rw_setting<bool, L"MemoryDumpAbortIfInsufficientMemory"> MemoryDumpAbortIfInsufficientMemory{ false };
        eil::single_threaded_rw_setting<bool, L"MemoryDumpIncludeNonessentialPages"> MemoryDumpIncludeNonessentialPages{ false };
        eil::single_threaded_rw_setting<bool, L"MemoryDumpCaptureUserPages"> MemoryDumpCaptureUserPages{ false };
        eil::single_threaded_rw_setting<bool, L"AppWindowAlwaysOnTop"> AppWindowAlwaysOnTop{ false };
        eil::single_threaded_rw_setting<bool, L"AppWindowMinimizeOnUse"> AppWindowMinimizeOnUse{ false };
        eil::single_threaded_rw_setting<bool, L"AppWindowHideWhenMinimized"> AppWindowHideWhenMinimized{ false };
    };

    struct UserSettings
    {
        static inline wil::single_threaded_property<AdvancedSettings> AdvancedSettings;
        static inline wil::single_threaded_property<AppearanceSettings> AppearanceSettings;
        static inline wil::single_threaded_property<GeneralSettings> GeneralSettings;
    };

    struct InternalSettings : eil::settings_base
    {
        static inline eil::single_threaded_rw_setting<int32_t, L"ProcessesNameColumnLastWidth"> ProcessesNameColumnLastWidth{ 250 };
        static inline eil::single_threaded_rw_setting<int32_t, L"ProcessesStatusColumnLastWidth"> ProcessesStatusColumnLastWidth{ 50 };
        static inline eil::single_threaded_rw_setting<int32_t, L"ProcessesPidColumnLastWidth"> ProcessesPidColumnLastWidth{ 50 };
        static inline eil::single_threaded_rw_setting<int32_t, L"ProcessesCpuUsageColumnLastWidth"> ProcessesCpuUsageColumnLastWidth{ 75 };
        static inline eil::single_threaded_rw_setting<int32_t, L"ProcessesIoRateColumnLastWidth"> ProcessesIoRateColumnLastWidth{ 75 };
        static inline eil::single_threaded_rw_setting<int32_t, L"ProcessesPrivateBytesLastWidth"> ProcessesPrivateBytesLastWidth{ 50 };
    };
}