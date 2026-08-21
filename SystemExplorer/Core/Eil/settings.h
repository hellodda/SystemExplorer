#ifdef __INTELLISENSE__
#include <wil/cppwinrt_authoring.h>
#include <winrt/SystemExplorer.Core.Data.Enums.h>
#endif
#include "event.h"

#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>

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


		struct wstring_hash
		{
			using is_transparent = void;
			size_t operator()(std::wstring_view v) const
			{
				return absl::Hash<std::wstring_view>{}(v);
			}
		};

		struct wstring_eq
		{
			using is_transparent = void;
			bool operator()(std::wstring_view lhs, std::wstring_view rhs) const
			{
				return lhs == rhs;
			}
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
			: value_(defaultValue)
		{
			context_ = details::settings_property_context::current_context;
		}

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


	struct settings_base : details::settings_property_context
	{
		settings_base()
			: propertySet_(winrt::Microsoft::Windows::Storage::ApplicationData::GetDefault().LocalSettings().Values())
		{
			details::settings_property_context::current_context = this;
		}

		std::shared_ptr<eil::event<eil::action_t<std::wstring_view, winrt::Windows::Foundation::IInspectable const&>>> SettingChanged = 
			std::make_shared<eil::event<eil::action_t<std::wstring_view, winrt::Windows::Foundation::IInspectable const&>>>();;

		[[nodiscard]] winrt::Windows::Foundation::IInspectable get(std::wstring_view key) const override
		{
			if (auto it = cache_.find(key); it != cache_.end())
			{
				return it->second;
			}

			try
			{
				if (auto value = propertySet_.TryLookup(key))
				{
					cache_.emplace(std::wstring{ key }, value);
					return value;
				}
			}
			catch (winrt::hresult_error const&) { }

			return nullptr;
		}

		void set(std::wstring_view name, winrt::Windows::Foundation::IInspectable const& value) override
		{
			cache_.insert_or_assign(std::wstring{ name }, value);

			dirtyKeys_.insert(std::wstring{ name });

			SettingChanged->invoke(name, value);
		}

		[[nodiscard]] std::vector<std::wstring_view> get_pending_changes() const
		{
			std::vector<std::wstring_view> pending;
			pending.reserve(dirtyKeys_.size());
			for (auto const& key : dirtyKeys_)
			{
				pending.push_back(key); 
			}
			return pending;
		}

		void commit_changes()
		{
			if (dirtyKeys_.empty()) return;

			for (auto const& key : dirtyKeys_)
			{
				try
				{
					auto it = cache_.find(key);
					if (it != cache_.end() && it->second)
					{
						propertySet_.Insert(key, it->second);
					}
					else
					{
						propertySet_.Remove(key);
					}
				}
				catch (winrt::hresult_error const&) { /* Пропускаем сбойные ключи */ }
				CATCH_LOG()
			}
			dirtyKeys_.clear();
		}

	private:
		winrt::Windows::Foundation::Collections::IPropertySet propertySet_;

		mutable absl::flat_hash_map<std::wstring, winrt::Windows::Foundation::IInspectable, details::wstring_hash, details::wstring_eq> cache_;
		absl::flat_hash_set<std::wstring, details::wstring_hash, details::wstring_eq> dirtyKeys_;
	};
}
