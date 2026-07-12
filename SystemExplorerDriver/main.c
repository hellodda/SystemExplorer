#include <ntddk.h>

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
	UNREFERENCED_PARAMETER(DriverObject);

	KseDriverCleanup(DriverObject);
	

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "SystemExplorerDriver: Unloading driver\n");
}

_Function_class_(DRIVER_INITIALIZE)
_IRQL_requires_(PASSIVE_LEVEL)
_IRQL_requires_same_
EXTERN_C NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath
)
{
	UNREFERENCED_PARAMETER(RegistryPath);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "SystemExplorerDriver: DriverEntry called\n");
	DriverObject->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}