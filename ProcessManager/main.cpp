#include <Windows.h>
#include <stdio.h>

#define SYNC_CREATE_PROC_EVENT L"Global\\CreateProcEvent"
#define SYNC_CLOSE_PROC_EVENT L"Global\\CloseProcEvent"
#define STARTING_PROCESS "C:\\Windows\\System32\\notepad.exe"
#define TIMEOUT_MS 1000
#define SPIN_COUNT 4000

CRITICAL_SECTION csProcessManagement;
volatile HANDLE hCreateProcEvent;
volatile HANDLE hCloseProcEvent;
volatile BOOLEAN work = TRUE;

volatile BOOLEAN isProcessStarted = FALSE;
PROCESS_INFORMATION processInformation;
STARTUPINFO startupInfo;

void CloseEvents()
{
	if (hCreateProcEvent != NULL) {
		CloseHandle(hCreateProcEvent);
	}
	if (hCloseProcEvent != NULL) {
		CloseHandle(hCloseProcEvent);
	}
}

void WaitForThreadsFinished(HANDLE hThread1, HANDLE hThread2)
{
	if (hThread1 != NULL) {
		WaitForSingleObject(hThread1, INFINITE);
		CloseHandle(hThread1);
	}
	if (hThread2 != NULL) {
		WaitForSingleObject(hThread2, INFINITE);
		CloseHandle(hThread2);
	}
}

void StartProcess()
{
	if (!isProcessStarted) {
		ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
		startupInfo.cb = sizeof(STARTUPINFO);
		if (CreateProcess(STARTING_PROCESS, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo, &processInformation) == 0) {
			printf("Unable to start process. Error code %d\n", GetLastError());
		}
		else {
			isProcessStarted = TRUE;
		}
	}
}

void StopProcess()
{
	if (isProcessStarted) {
		if (TerminateProcess(processInformation.hProcess, EXIT_SUCCESS) == 0) {
			printf("Unable to stop process. Error code %d\n", GetLastError());
		}
		isProcessStarted = FALSE;
		CloseHandle(processInformation.hProcess);
		CloseHandle(processInformation.hThread);
		CloseHandle(startupInfo.hStdError);
		CloseHandle(startupInfo.hStdInput);
		CloseHandle(startupInfo.hStdOutput);
	}
}

DWORD WINAPI CreateEventListener(LPVOID lpParameter)
{
	DWORD status;
	while (work) {
		status = WaitForSingleObject(hCreateProcEvent, TIMEOUT_MS);
		if (status == WAIT_OBJECT_0) {
			printf("Create signal received\n");
			EnterCriticalSection(&csProcessManagement);
			StartProcess();
			LeaveCriticalSection(&csProcessManagement);
		}
	}
	return 0;
}

DWORD WINAPI CloseEventListener(LPVOID lpParameter)
{
	DWORD status;
	while (work) {
		status = WaitForSingleObject(hCloseProcEvent, TIMEOUT_MS);
		if (status == WAIT_OBJECT_0) {
			printf("Close signal received\n");
			EnterCriticalSection(&csProcessManagement);
			StopProcess();
			LeaveCriticalSection(&csProcessManagement);
		}
	}
	return 0;
}

void NormalWork()
{
	HANDLE hCreateListener = CreateThread(NULL, 0, CreateEventListener, NULL, 0, NULL);
	HANDLE hCloseListener = CreateThread(NULL, 0, CloseEventListener, NULL, 0, NULL);

	if ((hCreateListener == NULL) || (hCloseListener == NULL)) {
		printf("Unable to start threads\n", GetLastError());
	}
	else {
		printf("Listening events...\nPress ENTER to stop\n", GetLastError());
		getchar();
	}
	work = FALSE;
	WaitForThreadsFinished(hCreateListener, NULL);
}

void main()
{
	InitializeCriticalSectionAndSpinCount(&csProcessManagement, SPIN_COUNT);
	hCreateProcEvent = OpenEventW(SYNCHRONIZE, FALSE, SYNC_CREATE_PROC_EVENT);
	hCloseProcEvent = OpenEventW(SYNCHRONIZE, FALSE, SYNC_CLOSE_PROC_EVENT);

	if ((hCreateProcEvent == NULL) || (hCloseProcEvent == NULL)) {
		printf("Unable to open events\n");
	}
	else {
		NormalWork();
	}
	CloseEvents();

	printf("Press ENTER to exit\n");
	getchar();
	return;
}