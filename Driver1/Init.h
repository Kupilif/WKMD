#ifndef __Init_h__
#define __Init_h__

#include <ntddk.h>
#include "DriverVariables.h"

typedef PCHAR TPsGetProcessImageFileName(PEPROCESS Process);

extern NTSTATUS PsLookupProcessByProcessId(HANDLE ProcessId, PEPROCESS *Process);
PCSTR GetProcessFileNameById(IN HANDLE handle);
void CreateProcessNotifyRoutine(_In_ HANDLE ParentId, _In_ HANDLE ProcessId, _Inout_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo);
void DriverUnload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);

#endif

