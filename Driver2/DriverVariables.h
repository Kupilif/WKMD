#ifndef __DriverVariables_h__
#define __DriverVariables_h__

#include <ntddk.h>

#define DRIVER_NAME "Driver2"
#define TRACKING_PROCESS "regedit.exe"

typedef struct _DriverVariables
{
	BOOLEAN isCallbackSet;
	LARGE_INTEGER cookie;
	ANSI_STRING asTrackingProcess;
} DriverVariables, *PDriverVariables;

void dvInitialize(PDEVICE_OBJECT pDeviceObject);
PDriverVariables GetDriverVariables(PDEVICE_OBJECT pDeviceObject);

#endif

