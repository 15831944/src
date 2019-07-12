#include "stdafx.h"

BOOL GetFileTimes(LPTSTR lpFilePath, FILE_TIMES* pft) {
	BOOL bRet = FALSE;
	HANDLE hFile = NULL;

	do {
		hFile = CreateFile(lpFilePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			break;

		FILETIME cft = { 0 }, mft = { 0 }, aft = { 0 };
		bRet = GetFileTime(hFile, &cft, &aft, &mft);
		if (FALSE == bRet)
			break;

		FileTimeToSystemTime(&cft, &(pft->ct));
		FileTimeToSystemTime(&aft, &(pft->at));
		FileTimeToSystemTime(&mft, &(pft->mt));

	} while (FALSE);

	if (hFile) {
		CloseHandle(hFile);
	}

	return bRet;
}

FILETIME SystemTime2Filetime(SYSTEMTIME ct) {
	FILETIME  ft;
	::SystemTimeToFileTime(&ct, &ft);
	return ft;
}


BOOL ChangeFileTimes(LPTSTR lpFilePath, FILE_TIMES* pft) {
	BOOL bRet = FALSE;
	HANDLE hFile = NULL;
	do {
		hFile = CreateFile(lpFilePath, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile) {
			break;
		}

		FILETIME cft = { 0 }, mft = { 0 }, aft = { 0 };

		cft = SystemTime2Filetime(pft->ct);
		aft = SystemTime2Filetime(pft->at);
		mft = SystemTime2Filetime(pft->mt);
		SetFileTime(hFile, &cft, &aft, &mft);
	} while (FALSE);

	if (hFile)
		CloseHandle(hFile);

	return bRet;
}