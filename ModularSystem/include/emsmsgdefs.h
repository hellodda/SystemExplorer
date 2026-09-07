#pragma once

typedef struct _EMSM_REGISTER_MODULE
{
	EMS_STATUS Status;
	GUID ModuleGuid;
	PCWSTR ModuleName;
	PCWSTR ModuleAuthor;
	PCWSTR ModuleDescription;
} EMSM_REGISTER_MODULE, *PEMSM_REGISTER_MODULE;

typedef struct _EMSM_CONTENT
{
	EMS_STATUS Status;
	PBYTE Content;
} EMSM_CONTENT, *PEMSM_CONTENT;