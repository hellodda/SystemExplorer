#pragma once

#include <absl/container/inlined_vector.h>
#include <absl/functional/any_invocable.h>

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
    struct fast_event
    {
        fast_event() = default;

        fast_event(const fast_event&) = delete;
        fast_event& operator=(const fast_event&) = delete;

        fast_event(fast_event&&) = default;
        fast_event& operator=(fast_event&&) = default;

        eil::event_token operator()(T const& handler)
        {
            int64_t current_token = next_token_++;
            handlers_.emplace_back(current_token, std::move(handler));
            return { current_token };
        }

        eil::event_token operator()(T&& handler)
        {
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
        absl::InlinedVector<std::pair<int64_t, T>, 2> handlers_;
    };

    template<typename T>
    struct event
    {
        eil::event_token operator()(T const& handler)
        {
            int64_t current_token = next_token_++;
            handlers_.emplace_back(current_token, std::move(handler));
            return { current_token };
        }

        eil::event_token operator()(T&& handler)
        {
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
		std::vector<std::pair<int64_t, T>> handlers_;
    };

    template<typename... TArgs>
	using action_t = std::function<void(TArgs...)>;

    template<typename TReturn, typename... TArgs>
    using function_t = std::function<TReturn(TArgs...)>;

    template<typename... TArgs>
    using faction_t = absl::AnyInvocable<void(TArgs...)>;

    template<typename TReturn, typename... TArgs>
    using ffunction_t = absl::AnyInvocable<TReturn(TArgs...)>;
}