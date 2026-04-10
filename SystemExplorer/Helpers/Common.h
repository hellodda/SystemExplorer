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
