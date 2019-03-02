#include "stdafx.h"

//�����ļ���д

/*****************************************
���ܣ���list��һ��name��path��remarkд���ļ���
��ʽ��
?lpName*lpPath*lpRemark?lpName*lpPath*lpRemark
*****************************************/
//BOOL WriteItemToFile(HANDLE hWrite, LPTSTR lpName, LPTSTR lpPath){
//
//	TCHAR szWriteData[DEFAULT_SIZE] = {0};
//	DWORD dwWriteSize = 0, dwRetWriteBytes = 0;
//
//	::wsprintf(szWriteData, TEXT("?%s*%s"), lpName, lpPath);
//	dwWriteSize = sizeof(TCHAR) * _tcsclen(szWriteData);
//
//	SetFilePointer(hWrite, NULL, NULL, FILE_END);
//
//	WriteFile(hWrite, szWriteData, dwWriteSize, &dwRetWriteBytes, NULL);
//
//	return TRUE;
//}
BOOL WriteItemToFile(HANDLE hWrite, LPTSTR lpName, LPTSTR lpPath, LPTSTR lpRemark) {

	TCHAR szWriteData[DEFAULT_SIZE] = { 0 };
	DWORD dwWriteSize = 0, dwRetWriteBytes = 0;

	::wsprintf(szWriteData, TEXT("?%s*%s*%s"), lpName, lpPath, lpRemark);
	dwWriteSize = sizeof(TCHAR) * _tcsclen(szWriteData);

	SetFilePointer(hWrite, NULL, NULL, FILE_END);

	WriteFile(hWrite, szWriteData, dwWriteSize, &dwRetWriteBytes, NULL);

	return TRUE;
}

//���ܣ�һ���Զ�ȡ�����ļ���������
BOOL ReadFromFile(LPTSTR lpFilePath, __out LPTSTR lpRetData) {

	HANDLE hReadFile = NULL;
	DWORD dwReadData = 0;
	DWORD dwFileSize = 0;

	do{
		hReadFile = CreateFile(lpFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hReadFile == INVALID_HANDLE_VALUE)
			break;

		dwFileSize = GetFileSize(hReadFile, NULL);

		ReadFile(hReadFile, lpRetData, dwFileSize,&dwReadData, NULL);

	}while(FALSE);

	if(hReadFile)
		CloseHandle(hReadFile);

	return TRUE;
} 


//���ܣ���ȡ������Ϣ�ĵ�һ���ļ���Ϣ
BOOL GetItemFromData(LPTSTR lpReadData, LPTSTR lpName, LPTSTR lpPath, LPTSTR lpRemark) {

	if(_tcslen(lpReadData) == 0){
		return FALSE;
	}

	TCHAR *posBegin = _tcschr(lpReadData, TEXT('?'));
	TCHAR *posStar1 = _tcschr(posBegin, TEXT('*'));
	TCHAR *posStar2 = _tcschr(posStar1 + 1, TEXT('*'));
	TCHAR *posEnd = _tcschr(posBegin + 1, TEXT('?'));
	
	if(posEnd){
		RtlZeroMemory(lpName, DEFAULT_SIZE);
		RtlZeroMemory(lpPath, DEFAULT_SIZE);
		RtlZeroMemory(lpRemark, DEFAULT_SIZE);

		_tcsncpy(lpName, posBegin + 1, (posStar1 - posBegin - 1));//strncpy����n��������ݲ�����
		_tcsncpy(lpPath, posStar1 + 1, (posStar2 - posStar1 - 1));
		_tcsncpy(lpRemark, posStar2 + 1, (posEnd - posStar2 - 1));
		
		_tcscpy(lpReadData, posEnd);
	}
	else{
		RtlZeroMemory(lpName, DEFAULT_SIZE);
		RtlZeroMemory(lpPath, DEFAULT_SIZE);
		RtlZeroMemory(lpRemark, DEFAULT_SIZE);

		_tcsncpy(lpName, posBegin + 1, (posStar1 - posBegin - 1));//strncpy����n��������ݲ�����
		_tcsncpy(lpPath, posStar1 + 1, (posStar2 - posStar1 - 1));
		_tcscpy(lpRemark, posStar2 + 1);

		lpReadData[0] = 0;
	}

	return TRUE;
};


//ɾ��һ��
BOOL DeleteAnItem(LPTSTR lpIniPath, LPTSTR lpName, LPTSTR lpPath, LPTSTR lpRemark){

	TCHAR *szReadData = new TCHAR [1024 * 1024];
	TCHAR szDeleteItem[DEFAULT_SIZE] = {0};
	RtlZeroMemory(szReadData, 1024 * 1024);

	ReadFromFile(lpIniPath, szReadData);

	while(_tcslen(szReadData)){
		::wsprintf(szDeleteItem, TEXT("?%s*%s*%s"), lpName, lpPath, lpRemark);

		TCHAR *pos = tcsistr(szReadData, szDeleteItem);
		if(pos == NULL){
			break;
		}

		int iLen = _tcslen(szDeleteItem);
		_tcscpy(pos, pos + iLen);

		HANDLE hWrite = CreateFile(lpIniPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hWrite == INVALID_HANDLE_VALUE)
			break;

		DWORD dwWriteSize = 0, dwRetWriteBytes = 0;
		dwWriteSize = _tcslen(szReadData) * sizeof(TCHAR);
		WriteFile(hWrite, szReadData, dwWriteSize, &dwRetWriteBytes, NULL);

		if(hWrite)
			CloseHandle(hWrite);

		break;
	}

	return TRUE;
}