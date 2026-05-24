#pragma once

#include <atomic>
#include <concepts>
#include <cstddef>
#include <deque>
#include <functional>
#include <future>
#include <semaphore>
#include <thread>
#include <tuple>  
#include <type_traits>

#ifdef __has_include
#    if __has_include(<version>)
#        include <version>
#    endif
#endif

#include "thread_safe_queue.h"

namespace native {
    namespace details
    {

#ifdef __cpp_lib_move_only_function
        using default_function_type = std::move_only_function<void()>;
#else
        using default_function_type = std::function<void()>;
#endif
    }  

    template <typename F, typename... Args>
    concept void_invocable = std::invocable<F, Args...>&& std::same_as<void, std::invoke_result_t<F, Args...>>;

    template <void_invocable FunctionType = details::default_function_type, typename ThreadType = std::jthread>
    class thread_pool
    {
        public:
            template <typename InitializationFunction = std::function<void(std::size_t)>>
                requires std::invocable<InitializationFunction, std::size_t>&&
            std::is_same_v<void, std::invoke_result_t<InitializationFunction, std::size_t>>
                explicit thread_pool(
                    const unsigned int& number_of_threads = std::thread::hardware_concurrency(),
                    InitializationFunction init = [](std::size_t) {})
                : tasks_(number_of_threads)
            {
                std::size_t current_id = 0;
                for (std::size_t i = 0; i < number_of_threads; ++i)
                {
                    priority_queue_.push_back(size_t(current_id));
                    try
                    {
                        threads_.emplace_back([&, id = current_id,
                            init](const std::stop_token& stop_tok) 
                            {
                                try
                                {
                                    std::invoke(init, id);
                                }
                                catch (...)
                                {

                                }

                                do 
                                {
                                    tasks_[id].signal.acquire();

                                    do 
                                    {
                                        while (auto task = tasks_[id].tasks.pop_front()) 
                                        {
                                            unassigned_tasks_.fetch_sub(1, std::memory_order_release);
                                            std::invoke(std::move(task.value()));
                                            in_flight_tasks_.fetch_sub(1, std::memory_order_release);
                                        }
                                        for (std::size_t j = 1; j < tasks_.size(); ++j)
                                        {
                                            const std::size_t index = (id + j) % tasks_.size();
                                            if (auto task = tasks_[index].tasks.steal()) 
                                            {
                                                unassigned_tasks_.fetch_sub(1, std::memory_order_release);
                                                std::invoke(std::move(task.value()));
                                                in_flight_tasks_.fetch_sub(1, std::memory_order_release);
                                                break;
                                            }
                                        }
                                    } while (unassigned_tasks_.load(std::memory_order_acquire) > 0);

                                    priority_queue_.rotate_to_front(id);

                                    if (in_flight_tasks_.load(std::memory_order_acquire) == 0) 
                                    {
                                        threads_complete_signal_.store(true, std::memory_order_release);
                                        threads_complete_signal_.notify_one();
                                    }

                                } while (!stop_tok.stop_requested());
                            });
                        ++current_id;

                    }
                    catch (...)
                    {
                        tasks_.pop_back();
                        std::ignore = priority_queue_.pop_back();
                    }
                }
            }

            ~thread_pool()
            {
                wait_for_tasks();
                for (std::size_t i = 0; i < threads_.size(); ++i) {
                    threads_[i].request_stop();
                    tasks_[i].signal.release();
                    threads_[i].join();
                }
            }

            thread_pool(const thread_pool&) = delete;
            thread_pool& operator=(const thread_pool&) = delete;

            [[nodiscard]] static thread_pool<>& global()
            {
                static thread_pool<> instance{};
                return instance;
            }

            template <typename Function, typename... Args,
                typename ReturnType = std::invoke_result_t<Function&&, Args&&...>>
                requires std::invocable<Function, Args...>
            [[nodiscard]] std::future<ReturnType> enqueue(Function f, Args... args)
            {
#ifdef __cpp_lib_move_only_function
                std::promise<ReturnType> promise;
                auto future = promise.get_future();
                auto task = [func = std::move(f), ... largs = std::move(args),
                    promise = std::move(promise)]() mutable
                    {
                    try
                    {
                        if constexpr (std::is_same_v<ReturnType, void>)
                        {
                            func(largs...);
                            promise.set_value();
                        }
                        else
                        {
                            promise.set_value(func(largs...));
                        }
                    }
                    catch (...) 
                    {
                        promise.set_exception(std::current_exception());
                    }
                    };
                enqueue_task(std::move(task));
                return future;
#else
           promise = std::move(promise)]() mutable {...};
                 */
                auto shared_promise = std::make_shared<std::promise<ReturnType>>();
                auto task = [func = std::move(f), ... largs = std::move(args),
                    promise = shared_promise]()
                    {
                    try
                    {
                        if constexpr (std::is_same_v<ReturnType, void>)
                        {
                            func(largs...);
                            promise->set_value();
                        }
                        else 
                        {
                            promise->set_value(func(largs...));
                        }

                    }
                    catch (...)
                    {
                        promise->set_exception(std::current_exception());
                    }
                    };

                auto future = shared_promise->get_future();
                enqueue_task(std::move(task));
                return future;
#endif
            }
            template <typename Function, typename... Args>
                requires std::invocable<Function, Args...>
            void enqueue_detach(Function&& func, Args&&... args)
            {
                enqueue_task(
                    std::move([f = std::forward<Function>(func),
                        ... largs = std::forward<Args>(args)]() mutable -> decltype(auto)
                        {
                            try
                            {
                                if constexpr (std::is_same_v<void,
                                    std::invoke_result_t<Function&&, Args&&...>>)
                                {
                                    std::invoke(f, largs...);
                                }
                                else
                                {
                                    std::ignore = std::invoke(f, largs...);
                                }
                            }
                            catch (...) {}
                        }));
            }
            [[nodiscard]] auto size() const { return threads_.size(); }

            void wait_for_tasks()
            {
                while (in_flight_tasks_.load(std::memory_order_acquire) > 0)
                {
                    threads_complete_signal_.wait(false);
                }
            }

            size_t clear_tasks()
            {
                size_t removed_task_count{ 0 };
                for (auto& task_list : tasks_) {
                    removed_task_count += task_list.tasks.clear();
                }
                in_flight_tasks_.fetch_sub(removed_task_count, std::memory_order_release);
                unassigned_tasks_.fetch_sub(removed_task_count, std::memory_order_release);

                return removed_task_count;
            }

        private:
            template <typename Function>
            void enqueue_task(Function&& f)
            {
                auto i_opt = priority_queue_.copy_front_and_rotate_to_back();
                if (!i_opt.has_value())
                {
                    return;
                }
                auto i = *(i_opt);

                unassigned_tasks_.fetch_add(1, std::memory_order_release);
                const auto prev_in_flight = in_flight_tasks_.fetch_add(1, std::memory_order_release);

                if (prev_in_flight == 0) 
                {
                    threads_complete_signal_.store(false, std::memory_order_release);
                }

                tasks_[i].tasks.push_back(std::forward<Function>(f));
                tasks_[i].signal.release();
            }

            struct task_item
            {
                native::thread_safe_queue<FunctionType> tasks{};
                std::binary_semaphore signal{ 0 };
            };

            std::vector<ThreadType> threads_;
            std::deque<task_item> tasks_;
            native::thread_safe_queue<std::size_t> priority_queue_;
            std::atomic_int_fast64_t unassigned_tasks_{ 0 }, in_flight_tasks_{ 0 };
            std::atomic_bool threads_complete_signal_{ false };
    };
}  