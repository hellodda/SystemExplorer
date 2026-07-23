#pragma once

#include <ksedyn.h>

_Must_inspect_result_
NTSTATUS KseDynDataLookup(
    _In_reads_bytes_(Length) PKSE_DYN_CONFIG Config,
    _In_ ULONG Length,
    _In_ USHORT Class,
    _In_ USHORT Machine,
    _In_ ULONG TimeDateStamp,
    _In_ ULONG SizeOfImage,
    _Outptr_opt_ PKSE_DYN_DATA* Data,
    _In_ ULONG FieldsLength,
    _Outptr_opt_result_buffer_(FieldsLength) PVOID* Fields
);
