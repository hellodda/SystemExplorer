#pragma once

EXTERN_C_START

#include "Native/se.h"
#include "Native/apiimport.h"
#include "Native/dltmgr.h"
#include "Native/error.h"
#include "Native/mapldr.h"
#include "Native/process.h"
#include "Native/procproc.h"
#include "Native/sebasesup.h"
#include "Native/guisup.h"
#include "Native/file.h"
#include "Native/symprv.h"
#include "Native/dump.h"


EXTERN_C_END

#include "../../../Common/wilx.h"


namespace native
{
    namespace details
    {
        inline void destroy_process_item(
            _In_  PSE_PROCESS_ITEM Item
        )
        {
            if (Item->ProcessName) free((PVOID)Item->ProcessName);
            if (Item->FileName) free((PVOID)Item->FileName);
            if (Item->CommandLine) free((PVOID)Item->CommandLine);
            if (Item->QueryHandle) NtClose(Item->QueryHandle);
        }

        struct process_item_deleter
        {
            void operator()(PSE_PROCESS_ITEM ptr) const
            {
                if (ptr) destroy_process_item(ptr);
            }
        };
    }

    using shared_process_item = std::shared_ptr<SE_PROCESS_ITEM>;
    using unique_process_item = std::unique_ptr<SE_PROCESS_ITEM, details::process_item_deleter>;
	using unique_nt_handle = wilx::unique_any<&NtClose>;

	constexpr inline bool is_valid(HANDLE handle)
	{
		return handle ? handle != INVALID_HANDLE_VALUE : false;
	}

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
    concept has_next_entry_offset = requires(T a)
    {
        { a.NextEntryOffset } -> std::convertible_to<ULONG>;
    };

    template <has_next_entry_offset T>
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

    template <has_next_entry_offset T>
    auto nt_make_range(T* head)
    {
        return std::ranges::subrange(nt_iterator<T>(head), nt_iterator<T>(nullptr));
    }

	namespace details
	{
		
	}
}