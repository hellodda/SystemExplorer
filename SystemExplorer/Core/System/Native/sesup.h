/*
 * Copyright (c) 2026 SystemExplorer, All rights reserved.
 *
 * This file is part of System Explorer.
 *
 * Authors:
 *
 *     hellodda 2026
 *
 */

#pragma once

#include "se.h"

#if defined(_cplusplus)
template <class T>
FORCEINLINE T* PTR_ADD_OFFSET(
    _In_ const T* Pointer,
    _In_ const T* Offset
)
{
    return reinterpret_cast<T*>(
        static_cast<const unsigned char*>(Pointer) +
        static_cast<const unsigned long long>(Offset)
        );
}

template <class T>
FORCEINLINE T* PTR_SUB_OFFSET(
    _In_ const T* Pointer,
    _In_ const T* Offset
)
{
    return reinterpret_cast<T*>(
        static_cast<const unsigned char*>(Pointer) -
        static_cast<const unsigned long long>(Pointer)
        );
}
#else
#define PTR_ADD_OFFSET(Pointer, Offset) ((PVOID)((PUCHAR)(Pointer) + (ULONG_PTR)(Offset)))
#define PTR_SUB_OFFSET(Pointer, Offset) ((PVOID)((PUCHAR)(Pointer) - (ULONG_PTR)(Offset)))
#endif

#define SE_LARGE_BUFFER_SIZE (256 * 1024 * 1024)

#define C_1uTo2(x) ((unsigned short)(unsigned char)(x))
#define C_1sTo2(x) ((unsigned short)(signed char)(x))
#define C_1uTo4(x) ((unsigned int)(unsigned char)(x))
#define C_1sTo4(x) ((unsigned int)(signed char)(x))
#define C_2uTo4(x) ((unsigned int)(unsigned short)(x))
#define C_2sTo4(x) ((unsigned int)(signed short)(x))
#define C_4uTo8(x) ((unsigned __int64)(unsigned int)(x))
#define C_4sTo8(x) ((unsigned __int64)(signed int)(x))

FORCEINLINE LONG_PTR __InterlockedExchangeAddPointer(
    _Inout_ _Interlocked_operand_ LONG_PTR volatile* Addend,
    _In_ LONG_PTR Value
)
{
#ifdef _WIN64
    return (LONG_PTR)_InterlockedExchangeAdd64((volatile LONG64*)Addend, (LONG64)Value);
#else
    return (LONG_PTR)_InterlockedExchangeAdd((volatile LONG*)Addend, (LONG)Value);
#endif
}

#define _InterlockedExchangeAddPointer __InterlockedExchangeAddPointer

FORCEINLINE
BOOLEAN __InterlockedBitTestAndSetPointer(
    _Inout_ _Interlocked_operand_ LONG_PTR volatile* Base,
    _In_ LONG_PTR Bit
)
{
#ifdef _WIN64
    return _interlockedbittestandset64((volatile LONG64*)Base, (LONG64)Bit);
#else
    return _interlockedbittestandset((volatile LONG*)Base, (LONG)Bit);
#endif
}

#define _InterlockedBitTestAndSetPointer __InterlockedBitTestAndSetPointer