#pragma once
#include <vector>
#include <string>

namespace eil
{
    inline std::vector<std::wstring> split_path(std::wstring_view path)
    {
        std::vector<std::wstring> result;

        std::wstring_view view{ path };
        size_t start = 0;

        while (start < view.size())
        {
            while (start < view.size() && view[start] == L'/')
                ++start;

            if (start >= view.size())
                break;

            size_t end = view.find(L'/', start);

            if (end == std::wstring_view::npos)
                end = view.size();

            result.emplace_back(view.substr(start, end - start));

            start = end;
        }

        return result;
    }

    template <size_t N>
    consteval std::string_view get_function_name(const char(&str)[N])
    {
        std::string_view s{ str, N - 1 };

        const auto pos = s.rfind("::");

        if (pos == std::string_view::npos)
            return s;

        return s.substr(pos + 2);
    }

    template <size_t N>
    consteval std::wstring_view get_function_namew(const wchar_t(&str)[N])
    {
        std::wstring_view s{ str, N - 1 };

        const auto pos = s.rfind(L"::");

        if (pos == std::wstring_view::npos)
            return s;

        return s.substr(pos + 2);
    }
}
#define get_current_function_name() eil::get_function_name(__FUNCTION__)
