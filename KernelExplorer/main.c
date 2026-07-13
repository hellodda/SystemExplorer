#include "kse.h"

PDRIVER_OBJECT KseDriverObject = { NULL };
RTL_OSVERSIONINFOEXW KseOsVersionInfo = { 0 };

_IRQL_requires_max_(PASSIVE_LEVEL)
VOID KseDriverCleanup(
	_In_ PDRIVER_OBJECT DriverObject
)
{

}

_Function_class_(DRIVER_UNLOAD)
_IRQL_requires_(PASSIVE_LEVEL)
_IRQL_requires_same_
VOID DriverUnload(
	_In_ PDRIVER_OBJECT DriverObject
)
{


	KseDriverCleanup(DriverObject);


	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "KSE: Unloading driver\n");
}

_Function_class_(DRIVER_INITIALIZE)
_IRQL_requires_(PASSIVE_LEVEL)
_IRQL_requires_same_
EXTERN_C NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath
)
{
	NTSTATUS status;

	KSE_PAGED_CODE_PASSIVE();

	KseDriverObject = DriverObject;
	KseDriverObject->DriverUnload = DriverUnload;

	KseOsVersionInfo.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);
	status = RtlGetVersion((PRTL_OSVERSIONINFOW)&KseOsVersionInfo);

	if (!NT_SUCCESS(status))
	{
		goto Exit;
	}


Exit:
}