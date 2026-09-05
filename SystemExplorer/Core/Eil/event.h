#pragma once

#ifndef EIL_ASSERT
#include <cassert>
#define EIL_ASSERT(expression) assert(expression)
#endif

namespace eil // explorer implementation library
{
    struct event_token
    {
        int64_t value{};

        explicit operator bool() const noexcept
        {
            return value != 0;
        }
    };

    template<typename T>
    struct event
    {
        event() = default;

        event(const event&) = delete;
        event& operator=(const event&) = delete;

        event(event&&) = default;
        event& operator=(event&&) = default;

        eil::event_token operator()(T const& handler)
        {
			EIL_ASSERT(handler);

            int64_t current_token = next_token_++;
            handlers_.emplace_back(current_token, std::move(handler));
            return { current_token };
        }

        eil::event_token operator()(T&& handler)
        {
			EIL_ASSERT(handler);

            int64_t current_token = next_token_++;
            handlers_.emplace_back(current_token, std::move(handler));
            return { current_token };
        }

        void operator()(eil::event_token const& token) noexcept
        {
            auto it = std::find_if(handlers_.begin(), handlers_.end(),
                [&](const auto& pair) { return pair.first == token.value; });

            if (it != handlers_.end())
            {
                if (it != handlers_.end() - 1)
                {
                    *it = std::move(handlers_.back());
                }
                handlers_.pop_back();
            }
        }

        template<typename... TArgs>
        auto invoke(TArgs&&... args) -> std::invoke_result_t<T, TArgs...>
        {
            using return_t = std::invoke_result_t<T, TArgs...>;

            if constexpr (std::is_void_v<return_t>)
            {
                for (auto& pair : handlers_)
                {
                    pair.second(args...);
                }
            }
            else
            {
                if (handlers_.empty())
                {
                    return return_t{};
                }
                for (size_t i = 0; i < handlers_.size() - 1; ++i)
                {
                    handlers_[i].second(args...);
                }
                return handlers_.back().second(args...);
            }
        }

    private:
        int64_t next_token_{ 1 };

#ifdef ABSL_CONTAINER_INLINED_VECTOR_H_
        absl::InlinedVector<std::pair<int64_t, T>, 2> handlers_;

#else
		std::vector<std::pair<int64_t, T>> handlers_;
#endif
    };

#ifdef  ABSL_FUNCTIONAL_ANY_INVOCABLE_H_
    template<typename... TArgs>
    using action_t = absl::AnyInvocable<void(TArgs...)>;

    template<typename TReturn, typename... TArgs>
    using function_t = absl::AnyInvocable<TReturn(TArgs...)>;
#else
    template<typename... TArgs>
    using action_t = std::function<void(TArgs...)>;

    template<typename TReturn, typename... TArgs>
    using function_t = std::function<TReturn(TArgs...)>;
#endif 
}