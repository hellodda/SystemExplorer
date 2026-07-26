#pragma once
#include "winrt_module_imports.h"
#include <variant>

namespace eil
{
	namespace details
	{
		template <size_t N>
		struct fixed_wstring
		{
			wchar_t buf[N]{};
			consteval fixed_string(const wchar_t(&str)[N])
			{
				std::copy_n(str, N, buf);
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
				auto value = winrt::box_value<T>(std::forward<Q>(q));
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

	template<typename TConfiguration = standard_settings_configuration>
	struct settings_base final : details::settings_property_context
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