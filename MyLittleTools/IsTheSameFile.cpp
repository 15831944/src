//头文件请自行添加
BOOL test_CmpFile(LPSTR szSaveFileName, LPSTR szOrgFile) {

	BOOL bRet = TRUE;

	char szOrgBuffer[1024 * 10] = { 0 };
	char szSaveBuffer[1024 * 10] = { 0 };
	HANDLE hOrgFile;
	HANDLE hSaveFile;
	do {
		hOrgFile = ::CreateFile(szOrgFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
		if (INVALID_HANDLE_VALUE == hOrgFile)
		{
			bRet = FALSE;
			break;
		}

		hSaveFile = ::CreateFile(szSaveFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
		if (INVALID_HANDLE_VALUE == hSaveFile)
		{
			bRet = FALSE;
			break;
		}

		LARGE_INTEGER liOrgFileSize = { 0 };
		LARGE_INTEGER liSaveFileSize = { 0 };
		liOrgFileSize.LowPart = ::GetFileSize(hOrgFile, (DWORD *)(&liOrgFileSize.HighPart));
		liSaveFileSize.LowPart = ::GetFileSize(hSaveFile, (DWORD *)(&liSaveFileSize.HighPart));

		if (liOrgFileSize.QuadPart != liSaveFileSize.QuadPart) {
			bRet = FALSE;
			break;
		}

		LARGE_INTEGER liReadFileSize = { 0 };

		DWORD dwReadDataLength = 1024 * 10;
		DWORD dwRet = 0;
		while (liReadFileSize.QuadPart < liOrgFileSize.QuadPart) {
			::SetFilePointer(hOrgFile, liReadFileSize.LowPart, &liReadFileSize.HighPart, FILE_BEGIN);
			::SetFilePointer(hSaveFile, liReadFileSize.LowPart, &liReadFileSize.HighPart, FILE_BEGIN);

			RtlZeroMemory(szOrgBuffer, dwReadDataLength);
			RtlZeroMemory(szSaveBuffer, dwReadDataLength);

			::ReadFile(hOrgFile, szOrgBuffer, dwReadDataLength, &dwRet, NULL);
			::ReadFile(hSaveFile, szSaveBuffer, dwReadDataLength, &dwRet, NULL);

			if (0 != strcmp(szOrgBuffer, szSaveBuffer)) {
				bRet = FALSE;
				break;
			}

			liReadFileSize.QuadPart += dwRet;
		}

	} while (FALSE);

	if(hSaveFile)
		CloseHandle(hSaveFile);
	if (hOrgFile)
		CloseHandle(hOrgFile);

	return bRet;
}


int main(){
	char szFile1[1024] = {0};
	char szFile2[1024] = {0};
	printf("Input the first file to cmp:\n");
	scanf("%s", szFile1);
	scanf("%s", szFile2);

	if(TRUE == test_CmpFile(szFile1, szFile2)){
		printf("Same.\n");
	}
	else
		printf("Diff.\n");
	system("pause");

	return 0;
}