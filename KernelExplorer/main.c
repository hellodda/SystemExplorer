#include "kse.h"
#include "ioctl.h"
#include "handler.h"

#define DEVICE_LINK_NAME   L"\\??\\KERNELEXPLORER"
#define DEVICE_OBJECT_NAME L"\\Device\\KERNELEXPLORER"

PDRIVER_OBJECT KseDriverObject = NULL;
RTL_OSVERSIONINFOEXW KseOsVersionInfo = { 0 };

_Function_class_(DRIVER_DISPATCH)
_IRQL_requires_max_(PASSIVE_LEVEL)
_IRQL_requires_same_
NTSTATUS KseCreateCloseDispatch(
	_In_ PDEVICE_OBJECT DeviceObject,
	_In_ PIRP Irp
)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	KSE_PAGED_CODE_PASSIVE();

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

_Function_class_(DRIVER_DISPATCH)
_IRQL_requires_max_(PASSIVE_LEVEL)
_IRQL_requires_same_
NTSTATUS IoControlDispatch(
	_In_ PDEVICE_OBJECT DeviceObject,
	_In_ PIRP Irp
)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	KSE_PAGED_CODE_PASSIVE();

	NTSTATUS Status = STATUS_INVALID_DEVICE_REQUEST; 
	ULONG_PTR BytesReturned = 0;                     

	PIO_STACK_LOCATION IoStackLocation = IoGetCurrentIrpStackLocation(Irp);
	ULONG IoControlCode = IoStackLocation->Parameters.DeviceIoControl.IoControlCode;
	ULONG InputLength = IoStackLocation->Parameters.DeviceIoControl.InputBufferLength;
	ULONG OutputLength = IoStackLocation->Parameters.DeviceIoControl.OutputBufferLength;

	if (IoControlCode == CTL_KSE_DISPATCH)
	{
		if (InputLength < sizeof(KSE_MESSAGE) || OutputLength < sizeof(KSE_MESSAGE))
		{
			Status = STATUS_BUFFER_TOO_SMALL;
			goto Complete;
		}

		PKSE_MESSAGE Message = (PKSE_MESSAGE)Irp->AssociatedIrp.SystemBuffer;

		if (!Message)
		{
			Status = STATUS_INVALID_PARAMETER;
			goto Complete;
		}

		BOOLEAN Handled = FALSE;
		for (ULONG i = 0; i < KseMessageHandlersCount; ++i)
		{
			if (KseMessageHandlers[i].MessageId == Message->Header.MessageId &&
				KseMessageHandlers[i].MessageHandler != NULL)
			{
				Status = KseMessageHandlers[i].MessageHandler(Message);

				BytesReturned = sizeof(KSE_MESSAGE);
				Handled = TRUE;
				break;
			}
		}

		if (!Handled)
		{
			Status = STATUS_INVALID_PARAMETER;
		}
	}

Complete:
	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = BytesReturned; 

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Status;
}

_IRQL_requires_max_(PASSIVE_LEVEL)
VOID KseDriverCleanup(
	_In_ PDRIVER_OBJECT DriverObject
)
{
	UNREFERENCED_PARAMETER(DriverObject);
	KSE_PAGED_CODE_PASSIVE();

	UNICODE_STRING DeviceLinkName;
	RtlInitUnicodeString(&DeviceLinkName, DEVICE_LINK_NAME);

	IoDeleteSymbolicLink(&DeviceLinkName);

	if (DriverObject->DeviceObject)
	{
		IoDeleteDevice(DriverObject->DeviceObject);
	}
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
	UNREFERENCED_PARAMETER(RegistryPath);

	NTSTATUS status;
	UNICODE_STRING DeviceObjectName;
	UNICODE_STRING DeviceLinkName;
	PDEVICE_OBJECT DeviceObject = NULL;

	KSE_PAGED_CODE_PASSIVE();

	RtlInitUnicodeString(&DeviceObjectName, DEVICE_OBJECT_NAME);
	RtlInitUnicodeString(&DeviceLinkName, DEVICE_LINK_NAME);

	status = IoCreateDevice(
		DriverObject,
		0,
		&DeviceObjectName,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&DeviceObject
	);

	if (!NT_SUCCESS(status))
	{
		KdPrint(("Device Object creation error\n"));
		return status;
	}

	status = IoCreateSymbolicLink(&DeviceLinkName, &DeviceObjectName);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(DeviceObject);
		return status;
	}

	KseDriverObject = DriverObject;
	KseDriverObject->DriverUnload = DriverUnload;

	KseDriverObject->MajorFunction[IRP_MJ_CREATE] = KseCreateCloseDispatch;
	KseDriverObject->MajorFunction[IRP_MJ_CLOSE] = KseCreateCloseDispatch;
	KseDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControlDispatch;

	KseOsVersionInfo.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);
	status = RtlGetVersion((PRTL_OSVERSIONINFOW)&KseOsVersionInfo);

	if (!NT_SUCCESS(status))
	{
		IoDeleteSymbolicLink(&DeviceLinkName);
		IoDeleteDevice(DeviceObject);
		return status;
	}

	DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

	return STATUS_SUCCESS;
}