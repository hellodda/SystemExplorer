#pragma once
#include "System.h"

namespace winrt::SystemExplorer::Core::System::Utils
{
    bool RestartProcess(
        _In_ HANDLE Process
    );
    std::wstring ExtractProcessName(
        _In_ PSYSTEM_PROCESS_INFORMATION pInfo,
        IN uint32_t pid
    );
    bool IsProcessSuspended(
        _In_ PSYSTEM_PROCESS_INFORMATION pInfo
    );

    inline ProcessStatus GetProcessStatus(
        _In_ PSYSTEM_PROCESS_INFORMATION pInfo
    )
    {
        return IsProcessSuspended(pInfo) ? ProcessStatus::Suspended : ProcessStatus::Running;
    }

    inline bool is_valid(
        _In_ HANDLE handle
    )
    {
        return handle != INVALID_HANDLE_VALUE;
    }

    // ntqueryinfo izi

    template <typename Func>
    auto nt_safe_wrapper(
        Func&& queryFunc,
        ULONG initialSize = 6400,
        ULONG maxSize = 12'800'000
    )
    {
        auto bufferSize = initialSize;

        std::unique_ptr<std::byte[]> buffer;

        for (auto attempts = 0; attempts < 10; ++attempts) 
        {
            buffer = std::make_unique_for_overwrite<std::byte[]>(bufferSize);
            
            ULONG returnLength{ 0 };
            auto status = queryFunc(buffer.get(), bufferSize, &returnLength);

            if (status == STATUS_INFO_LENGTH_MISMATCH || status == STATUS_BUFFER_OVERFLOW || status == STATUS_BUFFER_TOO_SMALL)
            {
                bufferSize = (returnLength > bufferSize) ? returnLength + 2048 : bufferSize * 2;

                if (bufferSize > maxSize) [[unlikely]]
                    throw std::bad_alloc();
                continue;
            }

            if (status < 0) [[unlikely]] 
                THROW_HR(HRESULT_FROM_NT(status));
            return buffer;
        }
        throw std::runtime_error("Failed to fetch NT buffer: too many resizes");
    }

    template<typename T>
    concept HasNextEntryOffset = requires(T a)
    {
        { a.NextEntryOffset } -> std::convertible_to<ULONG>;
    };

    template <HasNextEntryOffset T>
    struct nt_iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        nt_iterator() = default;
        explicit nt_iterator(T* ptr) : current_(ptr) {}

        reference operator*() const { return *current_; }
        pointer operator->() const { return current_; }

        nt_iterator& operator++()
        {
            if (current_ && current_->NextEntryOffset != 0)
                current_ = reinterpret_cast<T*>(reinterpret_cast<std::byte*>(current_) + current_->NextEntryOffset);
            else
                current_ = nullptr; 
            return *this;
        }

        nt_iterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }

        bool operator==(const nt_iterator& other) const { return current_ == other.current_; }
        bool operator!=(const nt_iterator& other) const { return !(*this == other); }
    private:
        T* current_{ nullptr };
    };

    template <HasNextEntryOffset T>
    auto nt_make_range(T* head)
    {
        return std::ranges::subrange(nt_iterator<T>(head), nt_iterator<T>(nullptr));
    }


}

