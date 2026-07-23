#pragma once

#include <ksemsg.h>

EXTERN_C_START

typedef struct _KSEM_STACK_TRACE
{
    USHORT Count;
    PVOID* Frames;
} KSEM_STACK_TRACE, * PKSEM_STACK_TRACE;

typedef struct _KSEM_SIZED_BUFFER
{
    USHORT Size;
    PBYTE Buffer;
} KSEM_SIZED_BUFFER, * PKSEM_SIZED_BUFFER;

//VOID KphMsgDynClear(
//    _Inout_ PKPH_MESSAGE Message
//);
//
//VOID KphMsgDynClearLast(
//    _Inout_ PKPH_MESSAGE Message
//);
//
//USHORT KphMsgDynRemaining(
//    _In_ PCKPH_MESSAGE Message
//);
//
//_Must_inspect_result_
//NTSTATUS KphMsgDynAddUnicodeString(
//    _Inout_ PKPH_MESSAGE Message,
//    _In_ KPH_MESSAGE_FIELD_ID FieldId,
//    _In_ PCUNICODE_STRING String
//);
//
//NTSTATUS KphMsgDynGetUnicodeString(
//    _In_ PCKPH_MESSAGE Message,
//    _In_ KPH_MESSAGE_FIELD_ID FieldId,
//    _Out_ PUNICODE_STRING String
//);
//
//_Must_inspect_result_
//NTSTATUS KphMsgDynAddAnsiString(
//    _Inout_ PKPH_MESSAGE Message,
//    _In_ KPH_MESSAGE_FIELD_ID FieldId,
//    _In_ PCANSI_STRING String
//);
//
//NTSTATUS KphMsgDynGetAnsiString(
//    _In_ PCKPH_MESSAGE Message,
//    _In_ KPH_MESSAGE_FIELD_ID FieldId,
//    _Out_ PANSI_STRING String
//);
//
//_Must_inspect_result_
//NTSTATUS KphMsgDynAddStackTrace(
//    _Inout_ PKPH_MESSAGE Message,
//    _In_ KPH_MESSAGE_FIELD_ID FieldId,
//    _In_ PKPHM_STACK_TRACE StackTrace
//);
//
//NTSTATUS KphMsgDynGetStackTrace(
//    _In_ PCKPH_MESSAGE Message,
//    _In_ KPH_MESSAGE_FIELD_ID FieldId,
//    _Out_ PKPHM_STACK_TRACE StackTrace
//);
//
//_Must_inspect_result_
//NTSTATUS KphMsgDynAddSizedBuffer(
//    _Inout_ PKPH_MESSAGE Message,
//    _In_ KPH_MESSAGE_FIELD_ID FieldId,
//    _In_ PKPHM_SIZED_BUFFER SizedBuffer
//);
//
//NTSTATUS KphMsgDynGetSizedBuffer(
//    _In_ PCKPH_MESSAGE Message,
//    _In_ KPH_MESSAGE_FIELD_ID FieldId,
//    _Out_ PKPHM_SIZED_BUFFER SizedBuffer
//);

EXTERN_C_END
