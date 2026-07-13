#pragma once
#include "kse.h"

_Function_class_(PFLT_POST_OPERATION_CALLBACK)
_IRQL_requires_max_(DISPATCH_LEVEL)
FLT_POSTOP_CALLBACK_STATUS FLTAPI KsepFltPostOp(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_In_opt_ PVOID CompletionContext,
	_In_ FLT_POST_OPERATION_FLAGS Flags
);

_Function_class_(PFLT_PRE_OPERATION_CALLBACK)
_IRQL_requires_max_(APC_LEVEL)
FLT_PREOP_CALLBACK_STATUS FLTAPI KsepFltPreOp(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Outptr_result_maybenull_ PVOID* CompletionContext
);

_IRQL_requires_max_(PASSIVE_LEVEL)
VOID KsepFltCleanupFileOp(
	VOID
);

_IRQL_requires_max_(PASSIVE_LEVEL)
VOID KsepFltInitializeFileOp(
	VOID
);

typedef enum _KSE_FLT_FILE_NAME_TYPE
{
	KseFltFileNameTypeNone,
	KseFltFileNameTypeNameInfo,
	KseFltFileNameTypeContext,
	KseFltFileNameTypeFileName,
	KseFltFileNameTypeNameCache
} KSE_FLT_FILE_NAME_TYPE, *PKSE_FLT_FILE_NAME_TYPE;

typedef struct _KSE_FLT_FILE_NAME_CACHE_ENTRY
{
	LIST_ENTRY ListEntry;
	PFILE_OBJECT FileObject;
	UNICODE_STRING FileName;
} KSE_FLT_FILE_NAME_CACHE_ENTRY, *PKSE_FLT_FILE_NAME_CACHE_ENTRY;

typedef struct _KSE_FLT_FILE_NAME
{
	KSE_FLT_FILE_NAME_TYPE Type;
	union
	{
		PFLT_FILE_NAME_INFORMATION NameInformation;
		PFLT_CONTEXT Context;
		PUNICODE_STRING FileName;
		PKSE_FLT_FILE_NAME_CACHE_ENTRY NameCache;
	};
} KSE_FLT_FILE_NAME, *PKSE_FLT_FILE_NAME;

#define KsepFltZeroFileName(info)				\
		(info)->Type = KseFltFileNameTypeNone;	\
		(info)->NameInformation = NULL;

_IRQL_requires_max_(APC_LEVEL)
_Must_inspect_result_
NTSTATUS KsepFltGetFileName(
	_In_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Out_ PKSE_FLT_FILE_NAME FltFileName
);

_IRQL_requires_max_(APC_LEVEL)
_Must_inspect_result_
NTSTATUS KsepFltGetDestFileName(
	_In_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Out_ PKSE_FLT_FILE_NAME FltDestFileName
);
_IRQL_requires_max_(APC_LEVEL)
VOID KsepFltReleaseFileName(
	_In_ KSE_FLT_FILE_NAME FltFileName
);

_IRQL_requires_max_(DISPATCH_LEVEL)
VOID KsepFiltReapFileNameCache(
	_In_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects
);

_IRQL_requires_max_(PASSIVE_LEVEL)
VOID KsepFltCleanupFileNameCache(
	VOID
);

_IRQL_requires_max_(PASSIVE_LEVEL)
VOID KsepFltInitializeFileNameCache(
	VOID
);