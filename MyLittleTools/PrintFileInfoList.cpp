// tmp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <vector>

BOOL PrintFileTimes(LPTSTR lpFilePath, const TCHAR *szBlank) {
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

		SYSTEMTIME st;
		TCHAR szMes[1024] = { 0 };
		FileTimeToSystemTime(&cft, &st);
		wsprintf(szMes, TEXT("  %s[CREATE TIME: %d-%d-%d %d:%d]"), szBlank, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);

		FileTimeToSystemTime(&aft, &st);
		wsprintf(szMes, TEXT("%s  [ACCESS TIME: %d-%d-%d %d:%d]"), szMes, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);

		FileTimeToSystemTime(&mft, &st);
		wsprintf(szMes, TEXT("%s  [MODIFY TIME: %d-%d-%d %d:%d]\n"), szMes, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);

		_tprintf(TEXT("%s"), szMes);
	} while (FALSE);

	if (hFile) {
		CloseHandle(hFile);
	}

	return bRet;
}

void GetFileList(LPTSTR szDir, const TCHAR * szBlank) {

	BOOL bNext = TRUE;
	TCHAR szPat[256] = { 0 };
	lstrcpy(szPat, szDir);
	lstrcat(szPat, TEXT("\\*.*"));

	WIN32_FIND_DATA wfd;
	HANDLE hFile = FindFirstFile(szPat, &wfd);
	while (hFile != NULL && bNext == TRUE) {

		if (lstrcmp(TEXT("."), wfd.cFileName) != 0 && lstrcmp(TEXT(".."), wfd.cFileName) != 0) {
			_tprintf(TEXT("%s%s——%s\\%s\n"), szBlank, wfd.cFileName, szDir, wfd.cFileName);
			TCHAR szFullPath[1024] = { 0 };
			wsprintf(szFullPath, TEXT("%s\\%s"), szDir, wfd.cFileName);

			if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
				TCHAR szSubBlank[1024] = { 0 };
				wsprintf(szSubBlank, TEXT("%s   "), szBlank);
				GetFileList(szFullPath, szSubBlank);
			}
			else {
				PrintFileTimes(szFullPath, szBlank);
			}
		}

		bNext = FindNextFile(hFile, &wfd);
	}
}
int main(int argc, char* argv[]) {

	TCHAR szFilePath[1024] = TEXT("");
	printf("input the root path:");
	_tscanf(TEXT("%[^\n]"), szFilePath);

	freopen("FileList.txt", "w", stdout);
	GetFileList(szFilePath, TEXT(""));

	freopen("CON", "w", stdout);
	printf("output to file : FileList.txt");
	system("pause");
	return 0;
}
