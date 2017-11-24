#ifndef __Init_h__
#define __Init_h__

#include <ntddk.h>
#include "DriverVariables.h"
#include "RegFilter.h"

#define DRIVER_NAME "Driver2"

typedef PCHAR TPsGetProcessImageFileName(PEPROCESS Process);

extern NTSTATUS PsLookupProcessByProcessId(HANDLE ProcessId, PEPROCESS *Process);
PCSTR GetProcessFileNameById(IN HANDLE handle);
NTSTATUS RegistryOperationsCallback(_In_ PVOID CallbackContext, _In_opt_ PVOID Argument1, _In_opt_ PVOID Argument2);
void DriverUnload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);

#endif

