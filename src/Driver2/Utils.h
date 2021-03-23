#ifndef __Utils_h__
#define __Utils_h__

#include <ntddk.h>

NTSTATUS OpenLogFile(PUNICODE_STRING fileName, PHANDLE handle);
NTSTATUS CloseLogFile(HANDLE handle);

#endif

