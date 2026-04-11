#pragma once

template <typename T, typename Pred>
int32_t IndexOf(winrt::Windows::Foundation::Collections::IVector<T> const& vector, Pred&& pred)
{
    for (uint32_t i = 0; i < vector.Size(); ++i)
    {
        if (pred(vector.GetAt(i)))
            return static_cast<int32_t>(i);
    }
    return -1;
}

inline std::wstring Format(const wchar_t* fmt, ...)
{
    if (!fmt) return L"";

    va_list args;
    va_start(args, fmt);

    int size = _vscwprintf(fmt, args);
    va_end(args);

    if (size <= 0) return L"";

    std::wstring result(size, L'\0');

    va_start(args, fmt);
    vswprintf_s(result.data(), result.size() + 1, fmt, args);
    va_end(args);

    return result;
}