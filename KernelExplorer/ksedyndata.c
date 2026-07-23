#include <ksebase.h>
#include <ksedyndata.h>

#ifndef _KERNEL_MODE
#include <ntintsafe.h>
#ifndef Add2Ptr
#define Add2Ptr(P,I) ((PVOID)((PUCHAR)(P) + (I)))
#endif
#endif

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
)
{
    NTSTATUS status;
    ULONG dataLength;
    ULONG length;

    if (Data)
    {
        *Data = NULL;
    }

    if (Fields)
    {
        *Fields = NULL;
    }

    status = RtlULongSub(Length,
        RTL_SIZEOF_THROUGH_FIELD(KSE_DYN_CONFIG, Count),
        &length);
    if (!NT_SUCCESS(status))
    {
        return STATUS_SI_DYNDATA_INVALID_LENGTH;
    }

    if (Config->Version != KPH_DYN_CONFIGURATION_VERSION)
    {
        return STATUS_SI_DYNDATA_VERSION_MISMATCH;
    }

    status = RtlULongMult(sizeof(KPH_DYN_DATA), Config->Count, &dataLength);
    if (!NT_SUCCESS(status))
    {
        return STATUS_SI_DYNDATA_INVALID_LENGTH;
    }

    status = RtlULongSub(length, dataLength, &length);
    if (!NT_SUCCESS(status))
    {
        return STATUS_SI_DYNDATA_INVALID_LENGTH;
    }

    for (ULONG i = 0; i < Config->Count; i++)
    {
        PKSE_DYN_DATA data;

        data = &Config->Data[i];

        if ((data->Class != Class) ||
            (data->Machine != Machine) ||
            (data->TimeDateStamp != TimeDateStamp) ||
            (data->SizeOfImage != SizeOfImage))
        {
            continue;
        }

        if (Fields)
        {
            PVOID start;
            ULONG end;

            status = RtlULongAdd(data->Offset, FieldsLength, &end);
            if (!NT_SUCCESS(status) || (end > length))
            {
                return STATUS_SI_DYNDATA_INVALID_LENGTH;
            }

            start = &Config->Data[Config->Count];

            *Fields = Add2Ptr(start, data->Offset);
        }

        if (Data)
        {
            *Data = data;
        }

        return STATUS_SUCCESS;
    }

    return STATUS_SI_DYNDATA_UNSUPPORTED_KERNEL;
}