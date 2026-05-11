#include "sebasesup.h"

_Use_decl_annotations_
PVOID SeAllocate(
    _In_ SIZE_T Size
)
{
    return RtlAllocateHeap(PhHeapHandle, HEAP_GENERATE_EXCEPTIONS, Size);
}

_Use_decl_annotations_
VOID SeFree(
    _In_opt_ _Frees_ptr_opt_ _Post_invalid_ PVOID Memory
)
{
    RtlFreeHeap(PhHeapHandle, 0, Memory);
}