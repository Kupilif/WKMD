#include "Init.h"

TPsGetProcessImageFileName *gpPsGetProcessImageFileName;
PDEVICE_OBJECT gpDeviceObject;

PCSTR GetProcessFileNameById(IN HANDLE handle)
{
	PEPROCESS Process;
	PsLookupProcessByProcessId(handle, &Process);
	return gpPsGetProcessImageFileName(Process);
}

void CreateProcessNotifyRoutine(_In_ HANDLE ParentId, _In_ HANDLE ProcessId, _Inout_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo)
{
	PCSTR procName = GetProcessFileNameById(ProcessId);
	ANSI_STRING asProcName;
	PDriverVariables driverVariables = GetDriverVariables(gpDeviceObject);
	BOOLEAN isStarted;

	RtlInitAnsiString(&asProcName, procName);
	if (CreateInfo != NULL) {
		isStarted = TRUE;
		DbgPrint("%s: %u %s started\n", DRIVER_NAME, ProcessId, asProcName.Buffer);
	}
	else {
		isStarted = FALSE;
		DbgPrint("%s: %u %s finished\n", DRIVER_NAME, ProcessId, asProcName.Buffer);
	}

	if (RtlEqualString(&asProcName, &(driverVariables->asTrackingProcess), FALSE)) {
		if (isStarted) {
			if (!driverVariables->isProcessStarted) {
				driverVariables->isProcessStarted = TRUE;
				driverVariables->hTrackingProcessId = ProcessId;
				if (driverVariables->isCreateProcEventCreated) {
					DbgPrint("%s: notification to start process\n", DRIVER_NAME);
					dvNotifyByEvent(driverVariables->createProcEvent);
				}
			}
		}
		else {
			if ((driverVariables->isProcessStarted) &&
					(driverVariables->hTrackingProcessId == ProcessId)) {
				driverVariables->isProcessStarted = FALSE;
				if (driverVariables->isCloseProcEventCreated) {
					DbgPrint("%s: notification to stop process\n", DRIVER_NAME);
					dvNotifyByEvent(driverVariables->closeProcEvent);
				}
			}
		}
	}
}

void DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
	DbgPrint("%s: driver unload routine\n", DRIVER_NAME);
	PDriverVariables driverVariables = GetDriverVariables(DriverObject->DeviceObject);

	if (driverVariables->isCallbackSet) {
		NTSTATUS status = PsSetCreateProcessNotifyRoutineEx(CreateProcessNotifyRoutine, TRUE);
		if (NT_SUCCESS(status)) {
			DbgPrint("%s: callback removed\n", DRIVER_NAME);
		}
		else {
			DbgPrint("%s: unable to remove callback\n", DRIVER_NAME);
		}
	}

	dvUninitialize(DriverObject->DeviceObject);
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

	UNICODE_STRING deviceName;
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT deviceObject = NULL;
	PDriverVariables driverVariables;

	DbgPrint("%s: driver entry routine\n", DRIVER_NAME);
	DbgPrint("%s: %ws\n", DRIVER_NAME, RegistryPath->Buffer);

	DriverObject->DriverUnload = DriverUnload;
	RtlInitUnicodeString(&deviceName, L"\\Device\\Driver1");
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

	status = PsSetCreateProcessNotifyRoutineEx(CreateProcessNotifyRoutine, FALSE);
	if (NT_SUCCESS(status)) {
		DbgPrint("%s: callback set\n", DRIVER_NAME);
		driverVariables->isCallbackSet = TRUE;
	}
	else {
		DbgPrint("%s: unable to set callback\n", DRIVER_NAME);
	}

	return STATUS_SUCCESS;
}