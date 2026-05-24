#pragma once
#include <random>

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

template<typename T>
winrt::Windows::Foundation::Collections::IVector<T> ViewToVector(winrt::Windows::Foundation::Collections::IVectorView<T> const& view) 
{
    if (!view)
        return nullptr;

    return winrt::single_threaded_vector(std::vector<T>(view.begin(), view.end()));
}

#include <winrt/Windows.Storage.h>

inline winrt::hstring SeGetCurrentAppXPath()
{
    return winrt::Windows::Storage::ApplicationData::Current().LocalFolder().Path();
}

inline std::wstring SeRandomString(int32_t length)
{
    WCHAR alphastring[16]{ L"" };
    const wchar_t charset[] = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(charset) / sizeof(wchar_t) - 2);

    for (int i = 0; i < length; ++i)
    {
        alphastring[i] = charset[dis(gen)];
    }
    alphastring[length] = L'\0';
}