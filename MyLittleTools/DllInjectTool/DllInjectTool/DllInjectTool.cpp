// DllInjectTool.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <tlhelp32.h>


//输出错误信息
LPTSTR ThrowMes(LPTSTR mes) {
	memset(mes, 0, _tcslen(mes) * sizeof(TCHAR));
	int err = GetLastError();
	//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), mes, 1024, NULL);
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 1033, mes, 1024, NULL);//1033是英语的language ID

	return mes;
}


//初始化目标进程的已知信息
void init(TCHAR *argv, TCHAR *ProcName, int *pid){

	if ((_tcsspn(argv, TEXT("0123456789")) == _tcslen(argv)) == 1) {
		_stscanf(argv, TEXT("%d"), pid);
	}
	else if (_tcscmp(argv, TEXT("*")) != 0) {
		_tcscpy(ProcName, argv);
	}
	return ;
}


typedef DWORD(WINAPI *PFNTCREATETHREADEX)
(
	PHANDLE                 ThreadHandle,
	ACCESS_MASK             DesiredAccess,
	LPVOID                  ObjectAttributes,
	HANDLE                  ProcessHandle,
	LPTHREAD_START_ROUTINE  lpStartAddress,
	LPVOID                  lpParameter,
	BOOL	                CreateSuspended,
	DWORD                   dwStackSize,
	DWORD                   dw1,
	DWORD                   dw2,
	LPVOID                  Unknown
	);


//直接调用NtCreateThreadEx，但这个在10.0内核版本的win10中已经不适用
BOOL MyCreateRemoteThread(HANDLE hProcess, LPTHREAD_START_ROUTINE pThreadProc, LPVOID pRemoteBuf){

	HANDLE hThread = NULL;

	DWORD pFunc = (DWORD)GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "NtCreateThreadEx");
	if (pFunc == NULL){
		return FALSE;
	}

	((PFNTCREATETHREADEX)pFunc)(&hThread,
		0x1FFFFF,
		NULL,
		hProcess,
		pThreadProc,
		pRemoteBuf,
		FALSE,
		NULL,
		NULL,
		NULL,
		NULL);
	if (hThread == NULL){
		return FALSE;
	}
	if (WAIT_FAILED == WaitForSingleObject(hThread, INFINITE)){
		return FALSE;
	}
	return TRUE;
}


//为防止重复注入，先判断是否已经注入
BOOL isThereDll(int pid, LPCTSTR dllPath){

	TCHAR mes[1024] = {0};

	LPTSTR name = (LPTSTR)_tcsrchr(dllPath, TEXT('\\'));


	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);
	if (hProcess == NULL) {
		_tprintf(TEXT("  [isThereDll error]OpenProcess : %s"), ThrowMes(mes));
		return FALSE;
	}

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (hSnap == NULL) {
		_tprintf(TEXT("  [isThereDll error]CreateToolhelp32Snapshot : %s"), ThrowMes(mes));
		CloseHandle(hProcess);
		return FALSE;
	}

	MODULEENTRY32 me = { sizeof(MODULEENTRY32) };
	BOOL tmp = Module32First(hSnap, &me);
	while (tmp) {

		if (_tcsicmp(me.szExePath, name + 1) == 0 || _tcsicmp(me.szModule, name + 1) == 0) {
			CloseHandle(hProcess);
			CloseHandle(hSnap);
			return TRUE;
		}

		tmp = Module32Next(hSnap, &me);
	}

	if (tmp == FALSE) {
		_tprintf(TEXT("  [isThereDll MES]no such mod : %s"), ThrowMes(mes));
		CloseHandle(hProcess);
		CloseHandle(hSnap);
		return FALSE;
	}
}


//注入函数
BOOL DllInject(int pid, LPCTSTR dllPath) {

	TCHAR mes[1024];
	HANDLE hProcess;

	if(TRUE == isThereDll(pid, dllPath)){
		_tprintf(TEXT("  [error]dll already injected.\n"));
		return FALSE;
	}

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);
	if (hProcess == NULL) {
		_tprintf(TEXT("  [inject error]OpenProcess : %s"), ThrowMes(mes));
		return FALSE;
	}

	LPVOID lpBuf = NULL;
	DWORD dwPathSize = _tcsclen(dllPath) * sizeof(TCHAR);
	lpBuf = VirtualAllocEx(hProcess, 0, dwPathSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (lpBuf == NULL) {
		_tprintf(TEXT("  [inject error]VirtualAllocEx : %s"), ThrowMes(mes));
		CloseHandle(hProcess);
		return FALSE;
	}

	BOOL temp = WriteProcessMemory(hProcess, lpBuf, dllPath, dwPathSize, NULL);
	if (temp == FALSE) {
		_tprintf(TEXT("  [inject error]WriteProcessMemory : %s"), ThrowMes(mes));
		CloseHandle(hProcess);
		return FALSE;
	}

	DWORD funAddr;
	if (sizeof(TCHAR) == 2)
		funAddr = (DWORD)GetProcAddress(GetModuleHandle(TEXT("KERNEL32.DLL")), "LoadLibraryW");
	else
		funAddr = (DWORD)GetProcAddress(GetModuleHandle(TEXT("KERNEL32.DLL")), "LoadLibraryA");

	if (funAddr == 0) {
		_tprintf(TEXT("  [inject error]GetProcAddress : %s"), ThrowMes(mes));
		CloseHandle(hProcess);
		return FALSE;
	}

	BOOL ret = MyCreateRemoteThread(hProcess, (LPTHREAD_START_ROUTINE)funAddr, lpBuf);
	if (ret == FALSE) {
		_tprintf(TEXT("  [inject error]MyCreateRemoteThread : %s"), ThrowMes(mes));
		CloseHandle(hProcess);
		return FALSE;
	}
	
	CloseHandle(hProcess);

	_tprintf(TEXT(" [inject succeeded.]\n"));
	return TRUE;
}


//卸载dll
BOOL DllDeject(int pid, LPTSTR dllName) {

	TCHAR mes[1024];

	LPTSTR name = _tcsrchr(dllName, TEXT('\\'));

	DWORD funAddr = (DWORD)GetProcAddress(GetModuleHandle(TEXT("KERNEL32.DLL")), "FreeLibrary");
	if (funAddr == 0) {
		_tprintf(TEXT("  [deject error]GetProcAddress : %s"), ThrowMes(mes));
		return FALSE;
	}

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);
	if (hProcess == NULL) {
		_tprintf(TEXT("  [deject error]OpenProcess : %s"), ThrowMes(mes));
		return FALSE;
	}

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (hSnap == NULL) {
		_tprintf(TEXT("  [deject error]CreateToolhelp32Snapshot : %s"), ThrowMes(mes));
		CloseHandle(hProcess);
		return FALSE;
	}

	MODULEENTRY32 me = { sizeof(MODULEENTRY32) };
	BOOL tmp = Module32First(hSnap, &me);
	while (tmp) {

		if (_tcsicmp(me.szExePath, name + 1) == 0 || _tcsicmp(me.szModule, name + 1) == 0) {
			_tprintf(TEXT("  [deject MES]FINDED MODULE : %s"), me.szModule);
			break;
		}

		tmp = Module32Next(hSnap, &me);
	}
	if (tmp == FALSE) {
		_tprintf(TEXT("  [deject error]FIND MOD FAILED : %s"), ThrowMes(mes));
		CloseHandle(hProcess);
		CloseHandle(hSnap);
		return FALSE;
	}


	BOOL ret = MyCreateRemoteThread(hProcess, (LPTHREAD_START_ROUTINE)funAddr, me.hModule);
	if (ret == FALSE) {
		_tprintf(TEXT("  [inject error]MyCreateRemoteThread : %s"), ThrowMes(mes));
		CloseHandle(hProcess);
		return FALSE;
	}

	CloseHandle(hProcess);
	CloseHandle(hSnap);

	_tprintf(TEXT(" [deject succeeded]\n"));

	return TRUE;
}


//遍历进程列表进行注入或卸载
void Proc_IDject_viaName(TCHAR *mod, TCHAR *ProcName, LPTSTR dllPath) {

	TCHAR mes[1024];
	TCHAR *name = NULL;
	_tprintf(TEXT("\n**********BEGIN*********\n"));

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == NULL) {
		_tprintf(TEXT("[error] CreateToolhelp32Snapshot : %s"), ThrowMes(mes));
		_tprintf(TEXT("\n***********END*********\n"));
		return;
	}

	PROCESSENTRY32 info = { sizeof(PROCESSENTRY32) };
	BOOL tmp = Process32First(hSnap, &info);
	if(tmp == FALSE){
		_tprintf(TEXT("[error] Process32First : %s"), ThrowMes(mes));
	}

	while (tmp) {

		if(_tcsicmp(ProcName, info.szExeFile) == 0 || _tcslen(ProcName) == 0){
			_tprintf(TEXT("\n%d-%s:\n"), info.th32ProcessID, info.szExeFile);

			if (_tcsicmp(mod, TEXT("-i")) == 0) {
				DllInject(info.th32ProcessID, dllPath);
			}
			else if (_tcsicmp(mod, TEXT("-d")) == 0) {
				DllDeject(info.th32ProcessID, dllPath);
			}

			if (_tcslen(ProcName) != 0)
				break;
		}

		tmp = Process32Next(hSnap, &info);
	}
	
	_tprintf(TEXT("\n***********END*********\n"));

	CloseHandle(hSnap);
	return;
}


int _tmain(int argc, TCHAR *argv[]){

	if(argc != 4){
		_tprintf(TEXT("pattern: (<pid>/<*>/<process name>) (<dll full path>) (<-i/-d>)"));
		return 0;
	}

	int pid = -1; 
	TCHAR ProcName[1024] = {0};

	//初始化：有名写名，有号写号，没有全要
	init(argv[1], ProcName, &pid);

	if(pid == -1){//使用进程名
		Proc_IDject_viaName(argv[3], ProcName, argv[2]);
	}
	else{//使用PID
		if (_tcsicmp(argv[3], TEXT("-i")) == 0) {
			DllInject(pid, argv[2]);
		}
		else if (_tcsicmp(argv[3], TEXT("-d")) == 0) {
			DllDeject(pid, argv[2]);
		}
	}
	
    return 0;
}

