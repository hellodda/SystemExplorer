#pragma once

#if __has_include(<phnt.h>)
#  include <phnt_windows.h>
#  include <phnt.h>
#endif

#ifndef EIL_ASSERT
#include <cassert>
#define EIL_ASSERT(expression) assert(expression)
#endif
#undef max

namespace eil::nt // explorer implementation library
{
    struct unicode_string_view;
    struct unicode_string;

    inline PUNICODE_STRING get_abi(unicode_string& object) noexcept;
    inline PUNICODE_STRING get_abi(unicode_string const& object) noexcept;
    inline PUNICODE_STRING get_abi(unicode_string_view& object) noexcept;
    inline PUNICODE_STRING get_abi(unicode_string_view const& object) noexcept;

    struct unicode_string_view
    {
        unsigned short length;
        unsigned short maximum_length;
        _Field_size_bytes_part_opt_(maximum_length, length) wchar_t* buffer;

        using value_type = wchar_t;
        using size_type = std::uint32_t;
        using reference = value_type&;
        using const_reference = value_type const&;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using iterator = const_pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        constexpr unicode_string_view(std::nullptr_t) = delete;

        constexpr unicode_string_view() noexcept
            : length(0), maximum_length(0), buffer(nullptr)
        {
        }

        constexpr unicode_string_view(_In_ PCUNICODE_STRING ustring) noexcept
            : length(ustring ? ustring->Length : 0),
            maximum_length(ustring ? ustring->MaximumLength : 0),
            buffer(ustring ? ustring->Buffer : nullptr)
        {
        }

        constexpr unicode_string_view(std::wstring_view string) noexcept
        {
            EIL_ASSERT(string.size() <= (std::numeric_limits<unsigned short>::max)() / sizeof(wchar_t));
            length = static_cast<unsigned short>(string.size() * sizeof(wchar_t));
            maximum_length = length;
            buffer = const_cast<wchar_t*>(string.data());
        }

        constexpr unicode_string_view(const wchar_t* string) noexcept
            : unicode_string_view(std::wstring_view(string))
        {
        }

        constexpr unicode_string_view(const wchar_t* string, size_type size) noexcept
            : unicode_string_view(std::wstring_view(string, size))
        {
        }

#if defined(WINRT_base_h) || defined(WINRT_Microsoft_Terminal_Core_H)
        unicode_string_view(winrt::hstring const& hstr) noexcept
            : unicode_string_view(std::wstring_view(hstr))
        {
        }
#endif

        constexpr operator std::wstring_view() const noexcept
        {
            if (buffer && length > 0)
            {
                return { buffer, static_cast<size_type>(length / sizeof(wchar_t)) };
            }
            return { L"", 0 };
        }

        constexpr const_reference operator[](size_type pos) const noexcept
        {
            EIL_ASSERT(pos < size());
            return *(begin() + pos);
        }

        constexpr const_reference front() const noexcept
        {
            EIL_ASSERT(!empty());
            return *begin();
        }

        constexpr const_reference back() const noexcept
        {
            EIL_ASSERT(!empty());
            return *(end() - 1);
        }

        constexpr const_pointer data() const noexcept { return buffer; }
        constexpr const_iterator begin() const noexcept { return buffer; }
        constexpr const_iterator cbegin() const noexcept { return begin(); }
        constexpr const_iterator end() const noexcept { return buffer ? buffer + size() : nullptr; }
        constexpr const_iterator cend() const noexcept { return end(); }
        constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
        constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        constexpr const_reverse_iterator crend() const noexcept { return rend(); }

        constexpr bool empty() const noexcept { return length == 0 || buffer == nullptr; }
        constexpr size_type size() const noexcept { return length / sizeof(wchar_t); }

        bool equals(unicode_string_view const& other, bool case_insensitive = false) const noexcept
        {
            return ::RtlCompareUnicodeString(get_abi(*this), get_abi(other), case_insensitive) == 0;
        }

        constexpr bool operator==(unicode_string_view const& other) const noexcept
        {
            return static_cast<std::wstring_view>(*this) == static_cast<std::wstring_view>(other);
        }

        constexpr bool operator==(std::wstring_view const& other) const noexcept
        {
            return static_cast<std::wstring_view>(*this) == other;
        }

#if __cpp_lib_starts_ends_with >= 201711L
        constexpr bool starts_with(wchar_t const value) const noexcept
        {
            return static_cast<std::wstring_view>(*this).starts_with(value);
        }
        constexpr bool starts_with(std::wstring_view const another) const noexcept
        {
            return static_cast<std::wstring_view>(*this).starts_with(another);
        }
        constexpr bool ends_with(wchar_t const value) const noexcept
        {
            return static_cast<std::wstring_view>(*this).ends_with(value);
        }
        constexpr bool ends_with(std::wstring_view const another) const noexcept
        {
            return static_cast<std::wstring_view>(*this).ends_with(another);
        }
#endif
    };

    struct unicode_string
    {
        unsigned short length;
        unsigned short maximum_length;
        _Field_size_bytes_part_opt_(maximum_length, length) wchar_t* buffer;

        using value_type = wchar_t;
        using size_type = std::uint32_t;
        using reference = value_type&;
        using const_reference = value_type const&;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        constexpr unicode_string(std::nullptr_t) = delete;
        unicode_string& operator=(std::nullptr_t) = delete;

        unicode_string() noexcept
            : length(0), maximum_length(0), buffer(nullptr)
        {
        }

        ~unicode_string() noexcept
        {
            clear();
        }

        unicode_string(unicode_string const& other)
            : length(0), maximum_length(0), buffer(nullptr)
        {
            assign(other.data(), other.size());
        }

        unicode_string& operator=(unicode_string const& other)
        {
            if (this != &other)
            {
                assign(other.data(), other.size());
            }
            return *this;
        }

        unicode_string(unicode_string&& other) noexcept
            : length(std::exchange(other.length, 0)),
            maximum_length(std::exchange(other.maximum_length, 0)),
            buffer(std::exchange(other.buffer, nullptr))
        {
        }

        unicode_string& operator=(unicode_string&& other) noexcept
        {
            if (this != &other)
            {
                clear();
                length = std::exchange(other.length, 0);
                maximum_length = std::exchange(other.maximum_length, 0);
                buffer = std::exchange(other.buffer, nullptr);
            }
            return *this;
        }

        explicit unicode_string(_In_ PCUNICODE_STRING ustring)
            : length(0), maximum_length(0), buffer(nullptr)
        {
            if (ustring && ustring->Buffer && ustring->Length > 0)
            {
                assign(ustring->Buffer, ustring->Length / sizeof(wchar_t));
            }
        }

        explicit unicode_string(unicode_string_view const& view)
            : length(0), maximum_length(0), buffer(nullptr)
        {
            assign(view.data(), view.size());
        }

        explicit unicode_string(std::wstring_view const& string)
            : length(0), maximum_length(0), buffer(nullptr)
        {
            assign(string.data(), string.size());
        }

        unicode_string(const wchar_t* string)
            : unicode_string(std::wstring_view(string))
        {
        }

#if defined(WINRT_BASE_H) || defined(WINRT_base_h)
        explicit unicode_string(winrt::hstring const& hstr)
            : unicode_string(std::wstring_view(hstr))
        {
        }

        operator winrt::hstring() const
        {
            if (empty()) return winrt::hstring{};
            return winrt::hstring(buffer, size());
        }
#endif

        void clear() noexcept
        {
            if (buffer)
            {
                ::RtlFreeHeap(NtCurrentPeb()->ProcessHeap, 0, buffer);
                buffer = nullptr;
            }
            length = 0;
            maximum_length = 0;
        }

        operator unicode_string_view() const noexcept
        {
            unicode_string_view view;
            view.length = length;
            view.maximum_length = maximum_length;
            view.buffer = buffer;
            return view;
        }

        operator std::wstring_view() const noexcept
        {
            if (buffer && length > 0)
            {
                return { buffer, static_cast<size_type>(length / sizeof(wchar_t)) };
            }
            return { L"", 0 };
        }

        reference operator[](size_type pos) noexcept
        {
            EIL_ASSERT(pos < size());
            return *(begin() + pos);
        }

        const_reference operator[](size_type pos) const noexcept
        {
            EIL_ASSERT(pos < size());
            return *(begin() + pos);
        }

        reference front() noexcept
        {
            EIL_ASSERT(!empty());
            return *begin();
        }

        const_reference front() const noexcept
        {
            EIL_ASSERT(!empty());
            return *begin();
        }

        reference back() noexcept
        {
            EIL_ASSERT(!empty());
            return *(end() - 1);
        }

        const_reference back() const noexcept
        {
            EIL_ASSERT(!empty());
            return *(end() - 1);
        }

        pointer data() noexcept { return buffer; }
        const_pointer data() const noexcept { return buffer; }
        iterator begin() noexcept { return buffer; }
        const_iterator begin() const noexcept { return buffer; }
        const_iterator cbegin() const noexcept { return begin(); }
        iterator end() noexcept { return buffer ? buffer + size() : nullptr; }
        const_iterator end() const noexcept { return buffer ? buffer + size() : nullptr; }
        const_iterator cend() const noexcept { return end(); }
        reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        const_reverse_iterator crbegin() const noexcept { return rbegin(); }
        reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        const_reverse_iterator crend() const noexcept { return rend(); }

        bool empty() const noexcept { return length == 0 || buffer == nullptr; }
        size_type size() const noexcept { return length / sizeof(wchar_t); }

        bool equals(unicode_string_view const& other, bool case_insensitive = false) const noexcept
        {
            return ::RtlCompareUnicodeString(get_abi(*this), get_abi(other), case_insensitive) == 0;
        }

        bool operator==(unicode_string_view const& other) const noexcept
        {
            return static_cast<std::wstring_view>(*this) == static_cast<std::wstring_view>(other);
        }

        bool operator==(std::wstring_view const& other) const noexcept
        {
            return static_cast<std::wstring_view>(*this) == other;
        }

        friend void swap(unicode_string& left, unicode_string& right) noexcept
        {
            std::swap(left.length, right.length);
            std::swap(left.maximum_length, right.maximum_length);
            std::swap(left.buffer, right.buffer);
        }

    private:
        void assign(_In_ const wchar_t* str_data, _In_ size_t str_size)
        {
            clear();
            if (str_size == 0) return;

            EIL_ASSERT(str_size <= ((std::numeric_limits<unsigned short>::max)() / sizeof(wchar_t)) - 1);

            length = static_cast<unsigned short>(str_size * sizeof(wchar_t));
            maximum_length = static_cast<unsigned short>((str_size + 1) * sizeof(wchar_t));

            buffer = static_cast<wchar_t*>(::RtlAllocateHeap(NtCurrentPeb()->ProcessHeap, 0, maximum_length));

            if (buffer)
            {
                if (str_data)
                {
                    std::copy_n(str_data, str_size, buffer);
                }
                buffer[str_size] = L'\0';
            }
            else
            {
                length = 0;
                maximum_length = 0;
            }
        }
    };

    inline PUNICODE_STRING get_abi(unicode_string& object) noexcept
    {
        return reinterpret_cast<PUNICODE_STRING>(&object);
    }

    inline PUNICODE_STRING get_abi(unicode_string const& object) noexcept
    {
        return const_cast<PUNICODE_STRING>(reinterpret_cast<PCUNICODE_STRING>(&object));
    }

    inline PUNICODE_STRING get_abi(unicode_string_view& object) noexcept
    {
        return reinterpret_cast<PUNICODE_STRING>(&object);
    }

    inline PUNICODE_STRING get_abi(unicode_string_view const& object) noexcept
    {
        return const_cast<PUNICODE_STRING>(reinterpret_cast<PCUNICODE_STRING>(&object));
    }

    inline void* detach_abi(unicode_string& object) noexcept
    {
        void* temp = object.buffer;
        object.length = 0;
        object.maximum_length = 0;
        object.buffer = nullptr;
        return temp;
    }

    inline void* detach_abi(unicode_string&& object) noexcept
    {
        return detach_abi(object);
    }
}

#ifdef __cpp_lib_format
template<>
struct std::formatter<eil::nt::unicode_string_view, wchar_t> : std::formatter<std::wstring_view, wchar_t>
{
    template <typename FormatContext>
    auto format(const eil::nt::unicode_string_view& str, FormatContext& ctx) const
    {
        return std::formatter<std::wstring_view, wchar_t>::format(static_cast<std::wstring_view>(str), ctx);
    }
};

template<>
struct std::formatter<eil::nt::unicode_string, wchar_t> : std::formatter<std::wstring_view, wchar_t>
{
    template <typename FormatContext>
    auto format(const eil::nt::unicode_string& str, FormatContext& ctx) const
    {
        return std::formatter<std::wstring_view, wchar_t>::format(static_cast<std::wstring_view>(str), ctx);
    }
};
#endif

template <>
struct std::hash<eil::nt::unicode_string_view>
{
    std::size_t operator()(const eil::nt::unicode_string_view& s) const noexcept
    {
        return std::hash<std::wstring_view>{}(static_cast<std::wstring_view>(s));
    }
};

template <>
struct std::hash<eil::nt::unicode_string>
{
    std::size_t operator()(const eil::nt::unicode_string& s) const noexcept
    {
        return std::hash<std::wstring_view>{}(static_cast<std::wstring_view>(s));
    }
};