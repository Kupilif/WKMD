#include "Utils.h"

NTSTATUS OpenLogFile(PUNICODE_STRING fileName, PHANDLE handle)
{

	OBJECT_ATTRIBUTES objectAttributes;
	IO_STATUS_BLOCK ioStatusBlock;

	InitializeObjectAttributes(&objectAttributes, fileName, OBJ_OPENIF, NULL, NULL);
	return ZwCreateFile(
		handle,
		FILE_APPEND_DATA,
		&objectAttributes,
		&ioStatusBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN_IF,
		FILE_NON_DIRECTORY_FILE,
		NULL,
		0
	);
}

NTSTATUS CloseLogFile(HANDLE handle)
{
	return ZwClose(handle);
}
