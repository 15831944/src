#include "stdafx.h"
#include <stdarg.h>

void MesBox(HWND hWnd, LPTSTR title, LPTSTR format,...) {
	TCHAR szTmp[DSIZE] = { 0 };

	va_list args = NULL;
	va_start(args, format);
	int iLen = _vsntprintf_s(szTmp, DSIZE, format, args);
	va_end(args);

	MessageBox(hWnd, szTmp, title, 0);
}