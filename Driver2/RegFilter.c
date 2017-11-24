#include "RegFilter.h"

LPCWSTR GetNotifyClassString(_In_ REG_NOTIFY_CLASS NotifyClass)
{
	switch (NotifyClass) {
	case RegNtPreDeleteKey:                 return L"RegNtPreDeleteKey";
	case RegNtPreSetValueKey:               return L"RegNtPreSetValueKey";
	case RegNtPreDeleteValueKey:            return L"RegNtPreDeleteValueKey";
	case RegNtPreSetInformationKey:         return L"RegNtPreSetInformationKey";
	case RegNtPreRenameKey:                 return L"RegNtPreRenameKey";
	case RegNtPreEnumerateKey:              return L"RegNtPreEnumerateKey";
	case RegNtPreEnumerateValueKey:         return L"RegNtPreEnumerateValueKey";
	case RegNtPreQueryKey:                  return L"RegNtPreQueryKey";
	case RegNtPreQueryValueKey:             return L"RegNtPreQueryValueKey";
	case RegNtPreQueryMultipleValueKey:     return L"RegNtPreQueryMultipleValueKey";
	case RegNtPreKeyHandleClose:            return L"RegNtPreKeyHandleClose";
	case RegNtPreCreateKeyEx:               return L"RegNtPreCreateKeyEx";
	case RegNtPreOpenKeyEx:                 return L"RegNtPreOpenKeyEx";
	case RegNtPreFlushKey:                  return L"RegNtPreFlushKey";
	case RegNtPreLoadKey:                   return L"RegNtPreLoadKey";
	case RegNtPreUnLoadKey:                 return L"RegNtPreUnLoadKey";
	case RegNtPreQueryKeySecurity:          return L"RegNtPreQueryKeySecurity";
	case RegNtPreSetKeySecurity:            return L"RegNtPreSetKeySecurity";
	case RegNtPreRestoreKey:                return L"RegNtPreRestoreKey";
	case RegNtPreSaveKey:                   return L"RegNtPreSaveKey";
	case RegNtPreReplaceKey:                return L"RegNtPreReplaceKey";

	case RegNtPostDeleteKey:                return L"RegNtPostDeleteKey";
	case RegNtPostSetValueKey:              return L"RegNtPostSetValueKey";
	case RegNtPostDeleteValueKey:           return L"RegNtPostDeleteValueKey";
	case RegNtPostSetInformationKey:        return L"RegNtPostSetInformationKey";
	case RegNtPostRenameKey:                return L"RegNtPostRenameKey";
	case RegNtPostEnumerateKey:             return L"RegNtPostEnumerateKey";
	case RegNtPostEnumerateValueKey:        return L"RegNtPostEnumerateValueKey";
	case RegNtPostQueryKey:                 return L"RegNtPostQueryKey";
	case RegNtPostQueryValueKey:            return L"RegNtPostQueryValueKey";
	case RegNtPostQueryMultipleValueKey:    return L"RegNtPostQueryMultipleValueKey";
	case RegNtPostKeyHandleClose:           return L"RegNtPostKeyHandleClose";
	case RegNtPostCreateKeyEx:              return L"RegNtPostCreateKeyEx";
	case RegNtPostOpenKeyEx:                return L"RegNtPostOpenKeyEx";
	case RegNtPostFlushKey:                 return L"RegNtPostFlushKey";
	case RegNtPostLoadKey:                  return L"RegNtPostLoadKey";
	case RegNtPostUnLoadKey:                return L"RegNtPostUnLoadKey";
	case RegNtPostQueryKeySecurity:         return L"RegNtPostQueryKeySecurity";
	case RegNtPostSetKeySecurity:           return L"RegNtPostSetKeySecurity";
	case RegNtPostRestoreKey:               return L"RegNtPostRestoreKey";
	case RegNtPostSaveKey:                  return L"RegNtPostSaveKey";
	case RegNtPostReplaceKey:               return L"RegNtPostReplaceKey";

	case RegNtCallbackObjectContextCleanup: return L"RegNtCallbackObjectContextCleanup";

	default:
		return L"Unsupported REG_NOTIFY_CLASS";
	}
}

BOOLEAN IsLogToFileNeed(_In_ REG_NOTIFY_CLASS NotifyClass)
{
	switch (NotifyClass) {
	case RegNtPostDeleteKey:
	case RegNtPostSetValueKey:
	case RegNtPostDeleteValueKey:
	case RegNtPostSetInformationKey:
	case RegNtPostRenameKey:
	case RegNtPostCreateKeyEx:
		return TRUE;

	default:
		return FALSE;
	}
}