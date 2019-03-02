#include "stdafx.h"

//������ʾ�ȴ�Ҷ��õĺ���

/***********************************************
���ܣ����������ת��Ϊ���֣�����ʾ�����ʽ��ʾ����
������
errStr : �Զ���ı���
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
��_tcsstr�����ϸĵĺ��Դ�Сд�汾
�÷���_tcsstrһ��
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

		while (*s1 && *s2 && (!(*s1 - *s2) || !(*s1 - *s2 - 32) || !(*s1 - *s2 + 32))) {//�޸�������
			s1++, s2++;
		}

		if (!*s2)
			return(cp);

		cp++;
	}

	return(NULL);

}