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

inline std::wstring Format(PCWSTR fmt, ...)
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

inline std::wstring NarrowToWide(PCSTR narrowStr)
{
    if (!narrowStr)
        return {};
    
    int size = MultiByteToWideChar(CP_UTF8, 0, narrowStr, -1, nullptr, 0);
    
    if (size <= 0)
        return {};
    
    std::wstring result(size - 1, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, narrowStr, -1, result.data(), size);
    
    return result;
}

inline constexpr std::wstring_view GetWilFailureTypeString(wil::FailureType type) noexcept
{
    switch (type)
    {
        case wil::FailureType::Exception: return L"Exception";
        case wil::FailureType::Return:    return L"Return";
        case wil::FailureType::Log:       return L"Log";
        case wil::FailureType::FailFast:  return L"FailFast";
        default:                          return L"Unknown";
    }
}