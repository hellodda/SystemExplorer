#pragma once

namespace eil // explorer implementation library
{
	template<typename T>
	struct delta
	{
		T value_{};
		T delta_{};

        constexpr delta() noexcept = default;

        constexpr delta(T value, T delta) noexcept :
            value_(value), delta_(delta)
        {
        }

        constexpr delta& operator+=(delta const& other)
        {
            value_ += other.value_;
            delta_ += other.delta_;
            return *this;
        }

        constexpr delta& operator-=(delta const& other)
        {
            value_ -= other.value_;
            delta_ -= other.delta_;
            return *this;
        }

        constexpr delta& operator*=(delta const& other)
        {
            value_ *= other.value_;
            delta_ *= other.delta_;
            return *this;
        }

        constexpr delta& operator/=(delta const& other)
        {
            value_ /= other.value_;
            delta_ /= other.delta_;
            return *this;
        }

        constexpr explicit operator bool() const noexcept
        {
            return delta_ > 0 && value_ > 0;
        }

        constexpr bool operator==(const delta&) const = default;
        constexpr auto operator<=>(const delta&) const = default;

        template<typename U>
        constexpr U narrow_delta() const
        {
            return gsl::narrow<U>(delta_);
        }

        template<typename U>
        constexpr U narrow_value() const
        {
            return gsl::narrow<U>(value_);
        }

        std::wstring to_string() const
        {
            return wil::str_printf<std::wstring>(L"[Delta:%d, Value:%d]", delta_, value_);
        }

        constexpr void update_deltas(T new_value)
        {
            delta_ = new_value - value_;
            value_ = new_value;
        }

#ifdef SYSX_Delta_H

        constexpr delta(SYSX_SINGLE_DELTA dlt) noexcept requires std::is_same_v<T, float> :
        value_(dlt.Value), delta_(dlt.Delta)
        {
        }

        constexpr operator SYSX_SINGLE_DELTA() const noexcept requires std::is_same_v<T, float>
        {
            return { value_, delta_ };
        }

        constexpr delta(SYSX_DOUBLE_DELTA dlt) noexcept requires std::is_same_v<T, double> :
        value_(dlt.Value), delta_(dlt.Delta)
        {
        }

        constexpr operator SYSX_DOUBLE_DELTA() const noexcept requires std::is_same_v<T, double>
        {
            return { value_, delta_ };
        }

        constexpr delta(SYSX_UINT32_DELTA dlt) noexcept requires (std::is_same_v<T, uint32_t> || std::is_same_v<T, unsigned long> || std::is_same_v<T, long>) :
            value_(static_cast<T>(dlt.Value)), delta_(static_cast<T>(dlt.Delta))
        {
        }

        constexpr operator SYSX_UINT32_DELTA() const noexcept requires (std::is_same_v<T, uint32_t> || std::is_same_v<T, unsigned long> || std::is_same_v<T, long>)
        {
            return { static_cast<ULONG>(value_), static_cast<ULONG>(delta_) };
        }

        constexpr delta(SYSX_UINT64_DELTA dlt) noexcept requires (std::is_same_v<T, uint64_t> || std::is_same_v<T, unsigned long long> || std::is_same_v<T, int64_t>) :
            value_(static_cast<T>(dlt.Value)), delta_(static_cast<T>(dlt.Delta))
        {
        }

        constexpr operator SYSX_UINT64_DELTA() const noexcept requires (std::is_same_v<T, uint64_t> || std::is_same_v<T, unsigned long long> || std::is_same_v<T, int64_t>)
        {
            return { static_cast<ULONG64>(value_), static_cast<ULONG64>(delta_) };
        }

        constexpr delta(SYSX_UINTPTR_DELTA dlt) noexcept requires std::is_same_v<T, uintptr_t> :
        value_(static_cast<T>(dlt.Value)), delta_(static_cast<T>(dlt.Delta))
        {
        }

        constexpr operator SYSX_UINTPTR_DELTA() const noexcept requires std::is_same_v<T, uintptr_t>
        {
            return { static_cast<ULONG_PTR>(value_), static_cast<ULONG_PTR>(delta_) };
        }
#endif

	};

    template<typename T, typename U>
    inline constexpr void update_native_delta(T* d, U v)
    {
        if (!d) [[unlikely]] return;

		d->Delta = v - d->Value;
		d->Value = v;
    }
}