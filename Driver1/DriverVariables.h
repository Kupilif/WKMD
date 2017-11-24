#ifndef __DriverVariables_h__
#define __DriverVariables_h__

#include <ntddk.h>

#define TRACKING_PROCESS "cmd.exe"
#define SYNC_CREATE_PROC_EVENT L"\\BaseNamedObjects\\CreateProcEvent"
#define SYNC_CLOSE_PROC_EVENT L"\\BaseNamedObjects\\CloseProcEvent"

typedef struct _DriverVariables
{
	BOOLEAN isCallbackSet;
	BOOLEAN isProcessStarted;
	BOOLEAN isCreateProcEventCreated;
	BOOLEAN isCloseProcEventCreated;
	HANDLE hTrackingProcessId;
	ANSI_STRING asTrackingProcess;
	PKEVENT createProcEvent;
	HANDLE createProcHandle;
	PKEVENT closeProcEvent;
	HANDLE closeProcHandle;
} DriverVariables, *PDriverVariables;

void dvInitialize(PDEVICE_OBJECT pDeviceObject);
PDriverVariables GetDriverVariables(PDEVICE_OBJECT pDeviceObject);
void dvUninitialize(PDEVICE_OBJECT pDeviceObject);
void dvNotifyByEvent(PKEVENT pEvent);

#endif

