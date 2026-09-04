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

_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS KseDispatchMessage(
	_In_ PKSE_MESSAGE Message,
	_In_ ULONG InputLength,
	_In_ ULONG OutputLength,
	_In_opt_ PETHREAD Thread,
	_Out_ PULONG_PTR BytesReturned
)
{
	UNREFERENCED_PARAMETER(InputLength);
	UNREFERENCED_PARAMETER(OutputLength);

	CLIENT_ID ClientId = { 0 };
	PKSE_MESSAGE_HANDLER Handler = KseMessageHandlers[Message->Header.MessageId];
	
	if (!Handler)
	{
		return STATUS_NOT_FOUND;
	}

	if (Thread)
	{
		ClientId.UniqueProcess = PsGetThreadProcessId(Thread);
		ClientId.UniqueThread = PsGetThreadId(Thread);
	}
	else
	{
		ClientId.UniqueProcess = PsGetCurrentProcessId();
		ClientId.UniqueThread = PsGetCurrentThreadId();
	}

	NTSTATUS Status = Handler(Message, &ClientId);
	*BytesReturned = Message->Header.Size > 0 ? Message->Header.Size : sizeof(KSE_MESSAGE);

	return Status;
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
		
		if (NT_SUCCESS(KseValidateMessage(Message)))
		{
			Status = KseDispatchMessage(
				Message,
				InputLength,
				OutputLength,
				Irp->Tail.Overlay.Thread,
				&BytesReturned
			);
		}
		else
		 goto Complete;
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

	NTSTATUS Status;
	UNICODE_STRING DeviceObjectName;
	UNICODE_STRING DeviceLinkName;
	PDEVICE_OBJECT DeviceObject = NULL;

	KSE_PAGED_CODE_PASSIVE();

	RtlInitUnicodeString(&DeviceObjectName, DEVICE_OBJECT_NAME);
	RtlInitUnicodeString(&DeviceLinkName, DEVICE_LINK_NAME);

	Status = IoCreateDevice(
		DriverObject,
		0,
		&DeviceObjectName,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&DeviceObject
	);

	if (!NT_SUCCESS(Status))
	{
		KdPrint(("KSE: Device Object creation error (0x%08X)\n", Status));
		return Status;
	}

	Status = IoCreateSymbolicLink(&DeviceLinkName, &DeviceObjectName);
	if (!NT_SUCCESS(Status))
	{
		KdPrint(("KSE: Symbolic Link creation error (0x%08X)\n", Status));
		IoDeleteDevice(DeviceObject);
		return Status;
	}

	KseDriverObject = DriverObject;
	KseDriverObject->DriverUnload = DriverUnload;

	KseDriverObject->MajorFunction[IRP_MJ_CREATE] = KseCreateCloseDispatch;
	KseDriverObject->MajorFunction[IRP_MJ_CLOSE] = KseCreateCloseDispatch;
	KseDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControlDispatch;

	KseOsVersionInfo.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);
	Status = RtlGetVersion((PRTL_OSVERSIONINFOW)&KseOsVersionInfo);

	if (!NT_SUCCESS(Status))
	{
		KdPrint(("KSE: Failed to get OS version (0x%08X)\n", Status));
		IoDeleteSymbolicLink(&DeviceLinkName);
		IoDeleteDevice(DeviceObject);
		return Status;
	}

	DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

	KdPrint(("KSE: Driver loaded successfully\n"));
	return STATUS_SUCCESS;
}