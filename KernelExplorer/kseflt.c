#include "kseflt.h"

_IRQL_requires_max_(APC_LEVEL)
_Must_inspect_result_
NTSTATUS KsepFltGetFileName(
    _In_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Out_ PKSE_FLT_FILE_NAME FltFileName
)
{
    NTSTATUS status;

    KSE_NPAGED_CODE_APC_MAX_FOR_PAGING_IO();

    NT_ASSERT(FltObjects->FileObject == Data->Iopb->TargetFileObject);

	KsepFltZeroFileName(FltFileName);

   
}