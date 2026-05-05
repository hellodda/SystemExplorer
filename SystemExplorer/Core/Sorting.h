#pragma once
#include <ranges>
#include <algorithm>
#include <compare>
#include <concepts>
#include <type_traits>

namespace winrt::SystemExplorer::Core::Sorting
{
    enum class SortDirection { Ascending, Descending };

    template <typename T>
    concept SortEnum = std::is_enum_v<T>;

    template <SortEnum TColumnEnum>
    class SortManager
    {
    public:
        explicit SortManager(TColumnEnum defaultColumn = static_cast<TColumnEnum>(0),
            SortDirection defaultDirection = SortDirection::Ascending)
            : currentColumn_(defaultColumn), direction_(defaultDirection) {}

        void Toggle(TColumnEnum column, SortDirection defaultNewDir = SortDirection::Descending)
        {
            if (currentColumn_ == column)
            {
                direction_ = (direction_ == SortDirection::Ascending)
                    ? SortDirection::Descending : SortDirection::Ascending;
            }
            else
            {
                currentColumn_ = column;
                direction_ = defaultNewDir;
            }
        }

        TColumnEnum Column() const noexcept { return currentColumn_; }
        bool IsAscending() const noexcept { return direction_ == SortDirection::Ascending; }

        template <std::ranges::random_access_range R, typename Comparator>
        void Sort(R& range, Comparator&& comp) const
        {
            if (currentColumn_ == static_cast<TColumnEnum>(0)) return; 

            auto sortAsc = IsAscending();
            auto col = currentColumn_;

            std::ranges::sort(range, [col, sortAsc, &comp](const auto& a, const auto& b) {
                auto result = comp(a, b, col);

                if (result == 0) return false;
                return sortAsc ? (result < 0) : (result > 0);
            });
        }

    private:
        TColumnEnum currentColumn_;
        SortDirection direction_;
    };
}
