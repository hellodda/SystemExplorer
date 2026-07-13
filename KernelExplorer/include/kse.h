#pragma once

#pragma once
#pragma warning(push)
#pragma warning(disable: 5103) // invalid preprocessing token (/Zc:preprocessor)
#include <ntifs.h>
#include <ntintsafe.h>
#include <minwindef.h>
#include <ntstrsafe.h>
#include <fltKernel.h>
#include <ntimage.h>
#include <bcrypt.h>
#include <usermode_accessors.h>
#pragma warning(pop)
//#include <pooltags.h>
#define PHNT_MODE PHNT_MODE_KERNEL
#include <phnt.h>
//#include <ntfill.h>
#include <ntpebteb.h>
#include <ntldr.h>
#include <ntwow64.h>
//#include <kphapi.h>
//#include <kphringbuff.h>


#define KSIAPI NTAPI

#define KSE_PAGED_CODE()                                                       \
    PAGED_CODE();                                                              \
    NT_ANALYSIS_ASSUME((KeGetCurrentIrql() == APC_LEVEL) ||                    \
                       (KeGetCurrentIrql() == PASSIVE_LEVEL))
#define KSE_PAGED_CODE_PASSIVE()                                               \
    PAGED_CODE();                                                              \
    NT_ASSERT(KeGetCurrentIrql() == PASSIVE_LEVEL);                            \
    NT_ANALYSIS_ASSUME(KeGetCurrentIrql() == PASSIVE_LEVEL)
#define KSE_PAGED_CODE_APC()                                                   \
    PAGED_CODE();                                                              \
    NT_ASSERT(KeGetCurrentIrql() == APC_LEVEL);                                \
    NT_ANALYSIS_ASSUME(KeGetCurrentIrql() == APC_LEVEL)
#define KSE_NPAGED_CODE_PASSIVE()                                              \
    NT_ASSERT(KeGetCurrentIrql() == PASSIVE_LEVEL);                            \
    NT_ANALYSIS_ASSUME(KeGetCurrentIrql() == PASSIVE_LEVEL)
#define KSE_NPAGED_CODE_APC_MAX()                                              \
    NT_ASSERT(KeGetCurrentIrql() <= APC_LEVEL);                                \
    NT_ANALYSIS_ASSUME((KeGetCurrentIrql() == APC_LEVEL) ||                    \
                       (KeGetCurrentIrql() == PASSIVE_LEVEL))
#define KSE_NPAGED_CODE_DISPATCH_MAX()                                         \
    NT_ASSERT(KeGetCurrentIrql() <= DISPATCH_LEVEL);                           \
    NT_ANALYSIS_ASSUME((KeGetCurrentIrql() == DISPATCH_LEVEL) ||               \
                       (KeGetCurrentIrql() == APC_LEVEL) ||                    \
                       (KeGetCurrentIrql() == PASSIVE_LEVEL))
#define KSE_NPAGED_CODE_DISPATCH_MIN()                                         \
    NT_ASSERT(KeGetCurrentIrql() >= DISPATCH_LEVEL);                           \
    NT_ANALYSIS_ASSUME((KeGetCurrentIrql() == DISPATCH_LEVEL) ||               \
                       (KeGetCurrentIrql() == HIGH_LEVEL))
#define KSE_NPAGED_CODE_DISPATCH()                                             \
    NT_ASSERT(KeGetCurrentIrql() == DISPATCH_LEVEL);                           \
    NT_ANALYSIS_ASSUME(KeGetCurrentIrql() == DISPATCH_LEVEL)
#define KSE_NPAGED_CODE_HIGH_MAX()                                             \
    NT_ASSERT(KeGetCurrentIrql() <= HIGH_LEVEL);                               \
    NT_ANALYSIS_ASSUME((KeGetCurrentIrql() == HIGH_LEVEL) ||                   \
                       (KeGetCurrentIrql() == DISPATCH_LEVEL) ||               \
                       (KeGetCurrentIrql() == APC_LEVEL) ||                    \
                       (KeGetCurrentIrql() == PASSIVE_LEVEL))

//
// N.B. This decorates code to indicate that the code supports up to APC_LEVEL
// but is in a non-paged segment since it can be called in a paging I/O path.
// Code in this path should also only allocate from non-paged pool to avoid
// deadlocks.
//
#define KSE_NPAGED_CODE_APC_MAX_FOR_PAGING_IO() KSE_NPAGED_CODE_APC_MAX()

#define KSE_PAGED_FILE()                                                       \
    __pragma(bss_seg("PAGEBBS"))                                               \
    __pragma(code_seg("PAGE"))                                                 \
    __pragma(data_seg("PAGEDATA"))                                             \
    __pragma(const_seg("PAGERO"))

#define KSE_PROTECTED_DATA_SECTION_PUSH()                                      \
    __pragma(data_seg(push))                                                   \
    __pragma(data_seg("KSIDATA"))
#define KSE_PROTECTED_DATA_SECTION_POP()                                       \
    __pragma(data_seg(pop))
#define KSE_PROTECTED_DATA_SECTION_RO_PUSH()                                   \
    __pragma(const_seg(push))                                                  \
    __pragma(const_seg("KSIRO"))
#define KSE_PROTECTED_DATA_SECTION_RO_POP()                                    \
    __pragma(const_seg(pop))

#define _Outptr_allocatesMem_ _Outptr_result_nullonfailure_ __drv_allocatesMem(Mem)
#define _Out_allocatesMem_ _Out_ __drv_allocatesMem(Mem)
#define _Out_allocatesMem_size_(size) _Out_allocatesMem_ _Post_writable_byte_size_(size)
#define _FreesMem_ _Pre_notnull_ _Post_ptr_invalid_ __drv_freesMem(Mem)
#define _In_freesMem_ _In_ _FreesMem_
#define _In_aliasesMem_ _In_ _Pre_notnull_ _Post_ptr_invalid_ __drv_aliasesMem
#define _Return_allocatesMem_ __drv_allocatesMem(Mem) _Check_return_ _Ret_maybenull_
#define _Return_allocatesMem_size_(size) _Return_allocatesMem_ _Post_writable_byte_size_(size)
#define _IRQL_requires_for_wait_(timeout)                                      \
    _When_(((timeout == NULL) || (timeout->QuadPart != 0)),                    \
           _IRQL_requires_max_(APC_LEVEL))                                     \
    _When_(((timeout != NULL) && (timeout->QuadPart == 0)),                    \
           _IRQL_requires_max_(DISPATCH_LEVEL))

typedef const void* PCVOID;
#ifndef MAX_PATH
#define MAX_PATH 260
#endif
