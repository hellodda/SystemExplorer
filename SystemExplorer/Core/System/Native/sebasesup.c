#include "sebasesup.h"

//NTSTATUS SeGetLastWin32ErrorAsNtStatus(
//    VOID
//)
//{
//    return SeDosErrorToNtStatus(SeGetLastError());
//}


BOOLEAN SeWriteUnicodeDecoder(
    _Inout_ PSE_UNICODE_DECODER Decoder,
    _In_ ULONG CodeUnit
)
{
    switch (Decoder->Encoding)
    {
    case SE_UNICODE_UTF8:
        if (Decoder->InputCount >= 4)
            return FALSE;
        Decoder->u.Utf8.Input[Decoder->InputCount] = (UCHAR)CodeUnit;
        Decoder->InputCount++;
        return TRUE;
    case SE_UNICODE_UTF16:
        if (Decoder->InputCount >= 2)
            return FALSE;
        Decoder->u.Utf16.Input[Decoder->InputCount] = (USHORT)CodeUnit;
        Decoder->InputCount++;
        return TRUE;
    case SE_UNICODE_UTF32:
        if (Decoder->InputCount >= 1)
            return FALSE;
        Decoder->u.Utf32.Input = CodeUnit;
        Decoder->InputCount = 1;
        return TRUE;
    default:
        //SeRaiseStatus(STATUS_UNSUCCESSFUL);
    }
}

ULONG64 SeReadTimeStampCounter(
    VOID
)
{
    MemoryBarrier();
    ULONG64 value = ReadTimeStampCounter();
    MemoryBarrier();
    return value;
}

BOOLEAN FASTCALL SefWaitForEvent(
    _Inout_ PSE_EVENT Event,
    _In_opt_ PLARGE_INTEGER Timeout
)
{
    BOOLEAN result;
    ULONG_PTR value;
    HANDLE eventHandle;

    value = ReadULongPtrAcquire(&Event->Value);

    if (value & SE_EVENT_SET)
        return TRUE;

    if (Timeout && Timeout->QuadPart == 0)
        return FALSE;

    SepReferenceEvent(Event);

    eventHandle = ReadPointerAcquire(&Event->EventHandle);

    if (!eventHandle)
    {
        OBJECT_ATTRIBUTES objectAttributes;
        InitializeObjectAttributes(&objectAttributes, NULL, OBJ_EXCLUSIVE, NULL, NULL);
        NtCreateEvent(&eventHandle, EVENT_ALL_ACCESS, &objectAttributes, NotificationEvent, FALSE);

        if (_InterlockedCompareExchangePointer(
            &Event->EventHandle,
            eventHandle,
            NULL
        ) != NULL)
        {
            NtClose(eventHandle);
            eventHandle = ReadPointerAcquire(&Event->EventHandle);
        }
    }
    if (!(ReadULongPtrAcquire(&Event->Value) & SE_EVENT_SET))
    {
        result = NtWaitForSingleObject(eventHandle, FALSE, Timeout) == STATUS_WAIT_0;
    }
    else
    {
        result = TRUE;
    }

    SepDereferenceEvent(Event, eventHandle);

    return result;
}

BOOLEAN FASTCALL SefBeginInitOnce(
    _Inout_ PSE_INITONCE InitOnce
)
{
    if (!_InterlockedBitTestAndSetPointer((PLONG_PTR)&InitOnce->Event.Value, SE_INITONCE_INITIALIZING_SHIFT))
        return TRUE;

    SeWaitForEvent(&InitOnce->Event, NULL);

    return FALSE;
}

VOID FASTCALL SefSetEvent(
    _Inout_ PSE_EVENT Event
)
{
    if (!_InterlockedBitTestAndSetPointer((PLONG_PTR)&Event->Value, SE_EVENT_SET_SHIFT))
    {
        HANDLE eventHandle;

        eventHandle = ReadPointerAcquire(&Event->EventHandle);

        if (eventHandle)
        {
            NtSetEvent(eventHandle, NULL);
        }

        SepDereferenceEvent(Event, eventHandle);
    }
}


VOID FASTCALL SefEndInitOnce(
    _Inout_ PSE_INITONCE InitOnce
)
{
    SeSetEvent(&InitOnce->Event);
}


_Use_decl_annotations_
PVOID SeAllocate(
    _In_ SIZE_T Size
)
{
    return RtlAllocateHeap(SeHeapHandle, HEAP_GENERATE_EXCEPTIONS, Size);
}

_Use_decl_annotations_
VOID SeFree(
    _In_opt_ _Frees_ptr_opt_ _Post_invalid_ PVOID Memory
)
{
    RtlFreeHeap(SeHeapHandle, 0, Memory);
}