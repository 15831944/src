#include "stdafx.h"

//错误提示等大家都用的函数

/***********************************************
功能：将错误代码转换为文字，以提示框的形式显示出来
参数：
errStr : 自定义的标题
***********************************************/
void ThrowMes(TCHAR *errStr) {

	TCHAR mes[1024];
	int err = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), mes, 1024, NULL);

#ifdef DEBUG
	MessageBox(NULL, mes, errStr, 0);
	_tprintf(TEXT("%s:%s\n"), errStr, mes);
#endif // DEBUG

}


/************************
在_tcsstr基础上改的忽略大小写版本
用法与_tcsstr一致
************************/
TCHAR * __cdecl tcsistr(
	const TCHAR * str1,
	const TCHAR * str2
)
{
	TCHAR *cp = (TCHAR *)str1;
	TCHAR *s1, *s2;

	if (!*str2)
		return((TCHAR *)str1);

	while (*cp)
	{
		s1 = cp;
		s2 = (TCHAR *)str2;

		while (*s1 && *s2 && (!(*s1 - *s2) || !(*s1 - *s2 - 32) || !(*s1 - *s2 + 32))) {//修改了这里
			s1++, s2++;
		}

		if (!*s2)
			return(cp);

		cp++;
	}

	return(NULL);

}