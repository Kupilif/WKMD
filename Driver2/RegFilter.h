#ifndef __RegFilter_h__
#define __RegFilter_h__

#include <ntddk.h>

LPCSTR GetNotifyClassString(_In_ REG_NOTIFY_CLASS NotifyClass);
BOOLEAN IsLogToFileNeed(_In_ REG_NOTIFY_CLASS NotifyClass);

#endif

