#include "Init.h"

TPsGetProcessImageFileName *gpPsGetProcessImageFileName;
PDEVICE_OBJECT gpDeviceObject;

PCSTR GetProcessFileNameById(IN HANDLE handle)
{
	PEPROCESS Process;
	PsLookupProcessByProcessId(handle, &Process);
	return gpPsGetProcessImageFileName(Process);
}

NTSTATUS RegistryOperationsCallback(_In_ PVOID CallbackContext, _In_opt_ PVOID Argument1, _In_opt_ PVOID Argument2)
{
	ANSI_STRING asProcName;
	HANDLE hCurrentProcId = PsGetCurrentProcessId();
	PCSTR procName = GetProcessFileNameById(hCurrentProcId);
	PDriverVariables driverVariables = GetDriverVariables(gpDeviceObject);
	REG_NOTIFY_CLASS notifyClass = (REG_NOTIFY_CLASS)(ULONG_PTR)Argument1;

	RtlInitAnsiString(&asProcName, procName);
	if (RtlEqualString(&asProcName, &(driverVariables->asTrackingProcess), FALSE)) {
		if (IsLogToFileNeed(notifyClass)) {
			DbgPrint("%s: %s: %ws\n", DRIVER_NAME, asProcName.Buffer, GetNotifyClassString(notifyClass));
		}
	}
	
	return STATUS_SUCCESS;
}

void DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
	DbgPrint("%s: driver unload routine\n", DRIVER_NAME);
	PDriverVariables driverVariables = GetDriverVariables(DriverObject->DeviceObject);

	if (driverVariables->isCallbackSet) {
		NTSTATUS status = CmUnRegisterCallback(driverVariables->cookie);
		if (NT_SUCCESS(status)) {
			DbgPrint("%s: callback removed\n", DRIVER_NAME);
		}
		else {
			DbgPrint("%s: unable to remove callback\n", DRIVER_NAME);
		}
	}

	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
	UNICODE_STRING usPsGetProcessImageFileName = RTL_CONSTANT_STRING(L"PsGetProcessImageFileName");
	gpPsGetProcessImageFileName = (TPsGetProcessImageFileName *)MmGetSystemRoutineAddress(&usPsGetProcessImageFileName);
	if (!gpPsGetProcessImageFileName)
	{
		DbgPrint("PSGetProcessImageFileName not found\n");
		return STATUS_UNSUCCESSFUL;
	}

	UNICODE_STRING deviceName, altitude;
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT deviceObject = NULL;
	PDriverVariables driverVariables;

	DbgPrint("%s: driver entry routine\n", DRIVER_NAME);
	DbgPrint("%s: %ws\n", DRIVER_NAME, RegistryPath->Buffer);

	DriverObject->DriverUnload = DriverUnload;
	RtlInitUnicodeString(&deviceName, L"\\Device\\Driver2");
	status = IoCreateDevice(DriverObject,
		sizeof(DriverVariables),
		&deviceName,
		FILE_DEVICE_UNKNOWN,
		0,
		FALSE,
		&deviceObject);
	if (!NT_SUCCESS(status)) {
		return status;
	}
	if (deviceObject == NULL) {
		return STATUS_UNEXPECTED_IO_ERROR;
	}
	gpDeviceObject = deviceObject;
	dvInitialize(deviceObject);
	driverVariables = GetDriverVariables(deviceObject);

	RtlInitUnicodeString(&altitude, L"100000");
	status = CmRegisterCallbackEx(
		RegistryOperationsCallback,
		&altitude,
		DriverObject,
		NULL,
		&(driverVariables->cookie),
		NULL);
	if (NT_SUCCESS(status)) {
		DbgPrint("%s: callback set\n", DRIVER_NAME);
		driverVariables->isCallbackSet = TRUE;
	}
	else {
		DbgPrint("%s: unable to set callback\n", DRIVER_NAME);
	}

	return STATUS_SUCCESS;
}