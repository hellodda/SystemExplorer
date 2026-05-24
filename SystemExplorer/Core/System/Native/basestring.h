#pragma once
#include "se.h"
#include "sebasesup.h"

NTSTATUS SeConvertUtf8ToUtf16Size(
    _Out_ PSIZE_T BytesInUtf16String,
    _In_reads_bytes_(BytesInUtf8String) PCCH Utf8String,
    _In_ SIZE_T BytesInUtf8String
)
{
    return STATUS_NOT_IMPLEMENTED;
}

_Use_decl_annotations_
PWSTR SeConvertUtf8ToUtf16Ex(
    _In_ PCCH Buffer,
    _In_ SIZE_T Length
)
{
    return nullptr;
}