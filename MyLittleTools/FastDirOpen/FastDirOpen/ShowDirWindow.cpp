#include "stdafx.h"

//显示特定的文件夹界面相关函数

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
BOOL g_bContinue = TRUE; //是否结束枚举的标志

						 /*****************************
						 功能：子窗口枚举回调函数
						 参数：
						 _In_ HWND hwnd:该子窗口的句柄
						 _In_ LPARAM lParam:参数,这里用来传递要查找的目标title
						 返回值：返回TRUE则EnumChildWindows就继续枚举，返回FALSE则EnumChildWindows结束枚举
						 *****************************/
BOOL CALLBACK FindlParamChildWindow(_In_ HWND hwnd, _In_ LPARAM lParam) {

	TCHAR szWindowTitle[MAX_BUF] = { 0 };
	TCHAR szTargetTitle[1024] = { 0 };
	_tcscpy_s(szTargetTitle, 1024, ((TARGETWND *)lParam)->szWndTitle);

	//1. 获取title
	GetWindowText(hwnd, szWindowTitle, MAX_BUF);

	//2. 查看title是否与lParam一致
	if (_tcslen(szWindowTitle)) {

		TCHAR *pos = tcsistr(szWindowTitle, szTargetTitle);

		if (pos && _tcslen(pos) == _tcslen(szTargetTitle)) {// explorer的子窗口中有一个title, eg:地址:c:\users\chris\desktop\py, 仅仅pos != NULL, 不行，比如lParam是c:\users\chris\desktop就找错了，所以还得验证长度

			_tprintf(TEXT("FIND CHILD : %s\n"), szWindowTitle);
			g_bContinue = FALSE;//如果找到目标title的子窗口就不再枚举该窗口其余子窗口
		}

	}

	return g_bContinue;
}


#include <psapi.h>
/*********************************
功能：检查窗口hwnd是否属于进程lpExeName
参数：
HWND hwnd:窗口句柄
LPTSTR lpExeName:进程名称
返回值：是则返回TRUE,不是则返回FALSE
*********************************/
BOOL CheckWndProc(HWND hwnd, LPTSTR lpExeName) {

	DWORD dwPid = 0;
	BOOL bRet = FALSE;

	//1. 获得hwnd对应的进程pid
	GetWindowThreadProcessId(hwnd, &dwPid);
	TCHAR szExeName[1024] = { 0 };
	TCHAR szExePath[1024] = { 0 };

	HANDLE hProcess = NULL;
	TCHAR *pos;

	//2. 比对进程名
	if (dwPid > 0) {
		do {
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);

			if (!hProcess) {
				break;
			}

			//2.1 获得进程路径
			GetModuleFileNameEx(hProcess, NULL, szExePath, 1024);

			//2.2 提取进程名称
			pos = _tcsrchr(szExePath, TEXT('\\'));
			if (!pos)
				pos = szExePath;

			_tcscpy_s(szExeName, 1024, pos + 1);

			//2.3 比较，如果一致就返回TRUE
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
功能：独立窗口枚举回调函数
参数：
_In_ HWND hwnd:窗口句柄
_In_ LPARAM lParam:参数，这里用于传递target title
返回值：返回TRUE则EnumWindows就继续枚举，返回FALSE则EnumWindows结束枚举
*********************************/
BOOL CALLBACK FindWindowsProc(_In_ HWND hwnd, _In_ LPARAM lParam) {

	TCHAR szWindowTitle[MAX_BUF] = { 0 };

	//1. 获取窗口名
	GetWindowText(hwnd, szWindowTitle, MAX_BUF);

	//2. 比较一下
	if (_tcscmp(szWindowTitle, ((TARGETWND *)lParam)->szWndTitle) == 0 && CheckWndProc(hwnd, ((TARGETWND *)lParam)->szProcName)) {
		ShowWindowAsync(hwnd, 9);
		SetForegroundWindow(hwnd);//如果没有弹出文件夹界面，说明该文件夹界面处于最小化状态，需要回车激活
		return FALSE;//本窗口就是目标窗口，结束枚举
	}

	//3. 查找子窗口是不是有对应标题
	EnumChildWindows(hwnd, FindlParamChildWindow, lParam);

	//4. 子窗口有对应标题
	if (g_bContinue == FALSE) {//说明这个程序的子窗口中有我要的路径信息，这个窗口就是我要找的文件夹窗口
							   //4.1 查看这个窗口是不是我要的进程的
		if (CheckWndProc(hwnd, ((TARGETWND *)lParam)->szProcName)) {//是目标进程的目标窗口
			ShowWindowAsync(hwnd, 9); //将窗口还原，否则最小化的窗口无法弹出
			SetForegroundWindow(hwnd);//如果没有弹出文件夹界面，说明该文件夹界面处于最小化状态，需要回车激活
		}
		else {//不是，则需要继续寻找
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

