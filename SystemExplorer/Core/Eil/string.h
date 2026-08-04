#pragma once
#include <vector>
#include <string>

namespace eil
{
    inline std::vector<std::wstring_view> split_path(std::wstring_view path)
    {
        std::vector<std::wstring_view> result;

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
}