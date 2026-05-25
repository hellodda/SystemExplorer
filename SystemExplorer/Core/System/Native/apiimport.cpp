#include "apiimport.h"
#include "sebasesup.h"
#include "mapldr.h"

FORCEINLINE
PVOID SepImportProcedure(
    _Inout_ PSE_INITONCE InitOnce,
    _Inout_ PVOID* Cache,
    _Inout_ PULONG_PTR Cookie,
    _In_ PCWSTR ModuleName,
    _In_ PCSTR ProcedureName
)
{
    if (SeBeginInitOnce(InitOnce))
    {
        PVOID module;
        PVOID procedure;

        module = SeGetLoaderEntryDllBaseZ(ModuleName);

        if (!module)
            module = SeLoadLibrary(ModuleName);

        if (module)
        {
            if (procedure = SeGetDllBaseProcedureAddress(module, ProcedureName, 0))
            {
                *Cookie = (ULONG_PTR)SeReadTimeStampCounter();
                *Cache = (PVOID)((ULONG_PTR)procedure ^ (ULONG_PTR)*Cookie);
            }
        }

        SeEndInitOnce(InitOnce);
    }

    if (*Cache && *Cookie)
        return (PVOID)((ULONG_PTR)*Cache ^ (ULONG_PTR)*Cookie);

    return NULL;
}

FORCEINLINE
PVOID SepImportProcedureNative(
    _Inout_ PSE_INITONCE InitOnce,
    _Inout_ PVOID* Cache,
    _Inout_ PULONG_PTR Cookie,
    _In_ PCWSTR ModuleName,
    _In_ PCSTR ProcedureName
)
{
    if (SeBeginInitOnce(InitOnce))
    {
        PVOID module;
        PVOID procedure;

        module = SeGetLoaderEntryDllBaseZ(ModuleName);

        if (!module)
            module = SeLoadLibrary(ModuleName);

        if (module)
        {
            ANSI_STRING procedureName;

            RtlInitAnsiString(&procedureName, ProcedureName);

            if (NT_SUCCESS(LdrGetProcedureAddress(
                module,
                &procedureName,
                0,
                &procedure
            )))
            {
                *Cookie = (ULONG_PTR)SeReadTimeStampCounter();
                *Cache = (PVOID)((ULONG_PTR)procedure ^ (ULONG_PTR)*Cookie);
            }
        }

        SeEndInitOnce(InitOnce);
    }

    if (*Cache && *Cookie)
        return (PVOID)((ULONG_PTR)*Cache ^ (ULONG_PTR)*Cookie);

    return NULL;
}

#define SE_DEFINE_IMPORT(Module, Name) \
typeof(&(Name)) Name##_Import(VOID) \
{ \
    static SE_INITONCE initOnce = SE_INITONCE_INIT; \
    static PVOID cache = NULL; \
    static ULONG_PTR cookie = 0; \
\
    return (typeof(&(Name)))SepImportProcedure(&initOnce, &cache, &cookie, (Module), (#Name)); \
}

#define SE_DEFINE_IMPORT_NATIVE(Module, Name) \
typeof(&(Name)) Name##_Import(VOID) \
{ \
    static SE_INITONCE initOnce = SE_INITONCE_INIT; \
    static PVOID cache = NULL; \
    static ULONG_PTR cookie = 0; \
\
    return (typeof(&(Name)))SepImportProcedureNative(&initOnce, &cache, &cookie, (Module), (#Name)); \
}

//SE_DEFINE_IMPORT(L"ntdll.dll", NtCreateThreadStateChange);
//SE_DEFINE_IMPORT(L"ntdll.dll", NtChangeProcessState);
//SE_DEFINE_IMPORT(L"ntdll.dll", NtCreateProcessStateChange);
//SE_DEFINE_IMPORT(L"ntdll.dll", NtSetInformationVirtualMemory);