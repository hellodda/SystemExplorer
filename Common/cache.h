#pragma once
#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>

namespace utils
{
    template <typename TKey, typename TItem>
    class cache_tracker
    {
    public:
        cache_tracker() = default;

        template <typename TPredicate, typename TFactory>
        TItem find_or_create(const TKey& key, TPredicate&& isValid, TFactory&& createNew)
        {
            auto it = cache_.find(key);

            if (it != cache_.end() && isValid(it->second))
            {
                return it->second;
            }

            TItem newItem = createNew();
            cache_[key] = newItem;
            return newItem;
        }

        template <typename TEvictCallback>
        void purge_inactive(const absl::flat_hash_set<TKey>& activeKeys, TEvictCallback&& onEvict)
        {
            absl::erase_if(cache_, [&activeKeys, &onEvict](const auto& pair) {
                if (!activeKeys.contains(pair.first))
                {
                    onEvict(pair.second);
                    return true;
                }
                return false;
            });
        }

        void purge_inactive(const absl::flat_hash_set<TKey>& activeKeys)
        {
            absl::erase_if(cache_, [&activeKeys](const auto& pair) {
                return !activeKeys.contains(pair.first);
            });
        }

        size_t size() const { return cache_.size(); }
        void clear() { cache_.clear(); }
    private:
        absl::flat_hash_map<TKey, TItem> cache_;
    };
}