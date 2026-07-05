#pragma once
#include "../../../../Common/wilx.h"
#include <Core/System/Native/procproc.h>
#include <memory>

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
}