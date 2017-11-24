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
	if (pDriverVariables == NULL) {
		return;
	}
	pDriverVariables->isCallbackSet = FALSE;
	RtlInitAnsiString(&(pDriverVariables->asTrackingProcess), TRACKING_PROCESS);
}