#ifndef __DriverVariables_h__
#define __DriverVariables_h__

#include <ntddk.h>

#define DRIVER_NAME "Driver2"
#define TRACKING_PROCESS "regedit.exe"
#define LOG_FILE_PATH L"\\??\\C:\\Driver2.log"
#define LOG_FILE_DELIMITER ":"

typedef struct _DriverVariables
{
	BOOLEAN isCallbackSet;
	LARGE_INTEGER cookie;
	ANSI_STRING asTrackingProcess;
	ANSI_STRING asDriverName;
	ANSI_STRING asLogFileDelimiter;
	ANSI_STRING asNewLineChar;
	UNICODE_STRING uslogFileName;
} DriverVariables, *PDriverVariables;

void dvInitialize(PDEVICE_OBJECT pDeviceObject);
PDriverVariables GetDriverVariables(PDEVICE_OBJECT pDeviceObject);

#endif

