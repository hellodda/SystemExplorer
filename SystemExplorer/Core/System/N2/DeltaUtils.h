#pragma once
#include <type_traits>

namespace native::deltamgr
{
    template <typename T>
    concept delta_concept = requires(T t)
    {
        requires std::is_arithmetic_v<decltype(t.Delta)>;
        requires std::is_arithmetic_v<decltype(t.Value)>;
    };

    template <delta_concept delta>
    static inline void Update(delta* deltaStruct, decltype(std::declval<delta>().Value) newValue)
    {
        if (!deltaStruct) [[unlikely]] return;

        deltaStruct->Delta = newValue - deltaStruct->Value;
        deltaStruct->Value = newValue;
    }
}