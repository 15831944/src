#include "stdafx.h"
#include <windows.h>


//输出错误信息
LPTSTR ThrowMes(LPTSTR mes, int err) {
	memset(mes, 0, _tcslen(mes) * sizeof(TCHAR));
	//int err = GetLastError();
	//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), mes, 1024, NULL);
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 1033, mes, 1024, NULL);//1033是英语的language ID

	if(_tcslen(mes) == 0)
		_tcscpy(mes, TEXT("[double err : no such error code]\n"));

	return mes;
}


int _tmain(){

	int err;

	TCHAR mes[1024];
	_tprintf(TEXT("input err code:"));

	while(_tscanf(TEXT("%d"), &err) == 1){
		_tprintf(TEXT(" =>  %s\n"), ThrowMes(mes, err));
		_tprintf(TEXT("input err code:"));
	}

	return 0;
}