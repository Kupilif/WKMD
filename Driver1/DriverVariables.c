#include "DriverVariables.h"

PDriverVariables GetDriverVariables(PDEVICE_OBJECT pDeviceObject) 
{
	if (pDeviceObject == NULL) {
		return NULL;
	}
	if (pDeviceObject->DeviceExtension == NULL) {
		return NULL;
	}
	else {
		return (PDriverVariables)pDeviceObject->DeviceExtension;
	}
}

void dvInitialize(PDEVICE_OBJECT pDeviceObject) 
{
	PDriverVariables pDriverVariables = GetDriverVariables(pDeviceObject);
	UNICODE_STRING uszProcessEventString;
	if (pDriverVariables == NULL) {
		return;
	}
	pDriverVariables->isCallbackSet = FALSE;
	pDriverVariables->isProcessStarted = FALSE;
	pDriverVariables->isCreateProcEventCreated = FALSE;
	pDriverVariables->isCloseProcEventCreated = FALSE;
	RtlInitAnsiString(&(pDriverVariables->asTrackingProcess), TRACKING_PROCESS);
	
	RtlInitUnicodeString(&uszProcessEventString, SYNC_CREATE_PROC_EVENT);
	pDriverVariables->createProcEvent = IoCreateNotificationEvent(&uszProcessEventString, &(pDriverVariables->createProcHandle));
	if (pDriverVariables->createProcEvent == NULL) {
		DbgPrint("%s: unable to create CREATE_PROC_EVENT", DRIVER_NAME);
	}
	else {
		KeClearEvent(pDriverVariables->createProcEvent);
		pDriverVariables->isCreateProcEventCreated = TRUE;
		DbgPrint("%s: created CREATE_PROC_EVENT", DRIVER_NAME);
	}

	RtlInitUnicodeString(&uszProcessEventString, SYNC_CLOSE_PROC_EVENT);
	pDriverVariables->closeProcEvent = IoCreateNotificationEvent(&uszProcessEventString, &(pDriverVariables->closeProcHandle));
	if (pDriverVariables->closeProcEvent == NULL) {
		DbgPrint("%s: unable to create CLOSE_PROC_EVENT", DRIVER_NAME);
	}
	else {
		KeClearEvent(pDriverVariables->closeProcEvent);
		pDriverVariables->isCloseProcEventCreated = TRUE;
		DbgPrint("%s: created CLOSE_PROC_EVENT", DRIVER_NAME);
	}
}

void dvUninitialize(PDEVICE_OBJECT pDeviceObject)
{
	PDriverVariables pDriverVariables = GetDriverVariables(pDeviceObject);
	if (pDriverVariables == NULL) {
		return;
	}
	if (pDriverVariables->isCreateProcEventCreated) {
		ZwClose(pDriverVariables->createProcHandle);
	}
	if (pDriverVariables->isCloseProcEventCreated) {
		ZwClose(pDriverVariables->closeProcEvent);
	}
}

void dvNotifyByEvent(PKEVENT pEvent)
{
	KeSetEvent(pEvent, 0, FALSE);
	KeClearEvent(pEvent);
}

HANDLE dvGetTrackingProcessId(PDEVICE_OBJECT pDeviceObject) {
	PDriverVariables pDriverVariables = GetDriverVariables(pDeviceObject);
	if (pDriverVariables == NULL) {
		return 0;
	}
	return pDriverVariables->hTrackingProcessId;
}

