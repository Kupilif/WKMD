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
	RtlInitUnicodeString(&(pDriverVariables->uslogFileName), LOG_FILE_PATH);
	pDriverVariables->isCallbackSet = FALSE;
	RtlInitAnsiString(&(pDriverVariables->asTrackingProcess), TRACKING_PROCESS);
	RtlInitAnsiString(&(pDriverVariables->asDriverName), DRIVER_NAME);
	RtlInitAnsiString(&(pDriverVariables->asLogFileDelimiter), LOG_FILE_DELIMITER);
	RtlInitAnsiString(&(pDriverVariables->asNewLineChar), "\n");
}