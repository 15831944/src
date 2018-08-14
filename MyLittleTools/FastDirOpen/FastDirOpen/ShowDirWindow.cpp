#include "stdafx.h"

//��ʾ�ض����ļ��н�����غ���

#define LAN_ID 1033
void ThrowMes(TCHAR *errStr, TCHAR *szMes) {

	TCHAR mes[1024] = { 0 };
	if (szMes == NULL) {
		int err = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, LAN_ID, mes, 1024, NULL);
		MessageBox(NULL, mes, errStr, 0);
	}
	else
		MessageBox(NULL, szMes, errStr, 0);
	//_tprintf(TEXT("%s:%s\n"), errStr, mes);
}


typedef struct wnd {
	TCHAR szProcName[1024];
	TCHAR szWndTitle[1024];
}TARGETWND;


#define MAX_BUF 1024
BOOL g_bContinue = TRUE; //�Ƿ����ö�ٵı�־

						 /*****************************
						 ���ܣ��Ӵ���ö�ٻص�����
						 ������
						 _In_ HWND hwnd:���Ӵ��ڵľ��
						 _In_ LPARAM lParam:����,������������Ҫ���ҵ�Ŀ��title
						 ����ֵ������TRUE��EnumChildWindows�ͼ���ö�٣�����FALSE��EnumChildWindows����ö��
						 *****************************/
BOOL CALLBACK FindlParamChildWindow(_In_ HWND hwnd, _In_ LPARAM lParam) {

	TCHAR szWindowTitle[MAX_BUF] = { 0 };
	TCHAR szTargetTitle[1024] = { 0 };
	_tcscpy_s(szTargetTitle, 1024, ((TARGETWND *)lParam)->szWndTitle);

	//1. ��ȡtitle
	GetWindowText(hwnd, szWindowTitle, MAX_BUF);

	//2. �鿴title�Ƿ���lParamһ��
	if (_tcslen(szWindowTitle)) {

		TCHAR *pos = tcsistr(szWindowTitle, szTargetTitle);

		if (pos && _tcslen(pos) == _tcslen(szTargetTitle)) {// explorer���Ӵ�������һ��title, eg:��ַ:c:\users\chris\desktop\py, ����pos != NULL, ���У�����lParam��c:\users\chris\desktop���Ҵ��ˣ����Ի�����֤����

			_tprintf(TEXT("FIND CHILD : %s\n"), szWindowTitle);
			g_bContinue = FALSE;//����ҵ�Ŀ��title���Ӵ��ھͲ���ö�ٸô��������Ӵ���
		}

	}

	return g_bContinue;
}


#include <psapi.h>
/*********************************
���ܣ���鴰��hwnd�Ƿ����ڽ���lpExeName
������
HWND hwnd:���ھ��
LPTSTR lpExeName:��������
����ֵ�����򷵻�TRUE,�����򷵻�FALSE
*********************************/
BOOL CheckWndProc(HWND hwnd, LPTSTR lpExeName) {

	DWORD dwPid = 0;
	BOOL bRet = FALSE;

	//1. ���hwnd��Ӧ�Ľ���pid
	GetWindowThreadProcessId(hwnd, &dwPid);
	TCHAR szExeName[1024] = { 0 };
	TCHAR szExePath[1024] = { 0 };

	HANDLE hProcess = NULL;
	TCHAR *pos;

	//2. �ȶԽ�����
	if (dwPid > 0) {
		do {
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);

			if (!hProcess) {
				break;
			}

			//2.1 ��ý���·��
			GetModuleFileNameEx(hProcess, NULL, szExePath, 1024);

			//2.2 ��ȡ��������
			pos = _tcsrchr(szExePath, TEXT('\\'));
			if (!pos)
				pos = szExePath;

			_tcscpy_s(szExeName, 1024, pos + 1);

			//2.3 �Ƚϣ����һ�¾ͷ���TRUE
			if (_tcsicmp(szExeName, lpExeName) == 0) {
				bRet = TRUE;
			}

		} while (FALSE);
	}

	if (hProcess) {
		CloseHandle(hProcess);
	}

	return bRet;
}


/*********************************
���ܣ���������ö�ٻص�����
������
_In_ HWND hwnd:���ھ��
_In_ LPARAM lParam:�������������ڴ���target title
����ֵ������TRUE��EnumWindows�ͼ���ö�٣�����FALSE��EnumWindows����ö��
*********************************/
BOOL CALLBACK FindWindowsProc(_In_ HWND hwnd, _In_ LPARAM lParam) {

	TCHAR szWindowTitle[MAX_BUF] = { 0 };

	//1. ��ȡ������
	GetWindowText(hwnd, szWindowTitle, MAX_BUF);

	//2. �Ƚ�һ��
	if (_tcscmp(szWindowTitle, ((TARGETWND *)lParam)->szWndTitle) == 0 && CheckWndProc(hwnd, ((TARGETWND *)lParam)->szProcName)) {
		ShowWindowAsync(hwnd, 9);
		SetForegroundWindow(hwnd);//���û�е����ļ��н��棬˵�����ļ��н��洦����С��״̬����Ҫ�س�����
		return FALSE;//�����ھ���Ŀ�괰�ڣ�����ö��
	}

	//3. �����Ӵ����ǲ����ж�Ӧ����
	EnumChildWindows(hwnd, FindlParamChildWindow, lParam);

	//4. �Ӵ����ж�Ӧ����
	if (g_bContinue == FALSE) {//˵�����������Ӵ���������Ҫ��·����Ϣ��������ھ�����Ҫ�ҵ��ļ��д���
							   //4.1 �鿴��������ǲ�����Ҫ�Ľ��̵�
		if (CheckWndProc(hwnd, ((TARGETWND *)lParam)->szProcName)) {//��Ŀ����̵�Ŀ�괰��
			ShowWindowAsync(hwnd, 9); //�����ڻ�ԭ��������С���Ĵ����޷�����
			SetForegroundWindow(hwnd);//���û�е����ļ��н��棬˵�����ļ��н��洦����С��״̬����Ҫ�س�����
		}
		else {//���ǣ�����Ҫ����Ѱ��
			g_bContinue = TRUE;
		}
	}

	return g_bContinue;
}


BOOL ShowTargetWnd(LPTSTR lpProcName, LPTSTR lpWndTitle) {

	BOOL bRet = FALSE;

	TARGETWND targetWnd;

	_tcscpy_s(targetWnd.szProcName, 1024, lpProcName);
	_tcscpy_s(targetWnd.szWndTitle, 1024, lpWndTitle);

	EnumWindows(FindWindowsProc, (LPARAM)(&targetWnd));

	return bRet;
}


BOOL ShowDirAlways(LPTSTR szPath) {

	BOOL bRet = FALSE;

	DWORD dwPid = 0;

	ShowTargetWnd(TEXT("explorer.exe"), szPath);

	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;

	if (g_bContinue) {
		TCHAR szCmd[1024] = { 0 };
		::wsprintf(szCmd, TEXT("explorer.exe \"%s\""), szPath);

		STARTUPINFO si = { sizeof(STARTUPINFO) };
		PROCESS_INFORMATION pi;
		bRet = ::CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	}
	else{
		bRet = TRUE;
	}

	return bRet;
}

