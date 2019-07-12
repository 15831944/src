#include "stdafx.h"
#include <stdarg.h>

int MesBox(HWND hWnd, int iMode, LPTSTR title, LPTSTR format,...) {
	TCHAR szTmp[DSIZE] = { 0 };

	va_list args = NULL;
	va_start(args, format);
	int iLen = _vsntprintf_s(szTmp, DSIZE, format, args);
	va_end(args);
	
	return MessageBoxEx(hWnd, szTmp, title, iMode, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
}

void ClearMallocVec(vector<LPTSTR> *vecList) {
	vector<LPTSTR>::iterator it;
	while (!vecList->empty()) {
		it = vecList->end() - 1;
		free(*it);
		vecList->pop_back();
	}
}

void ClearNewVec(vector<LPTSTR> *vecList) {
	vector<LPTSTR>::iterator it;
	while (!vecList->empty()) {
		it = vecList->end() - 1;
		delete[] (*it);
		vecList->pop_back();
	}
}