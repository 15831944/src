
// FastDirOpen.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFastDirOpenApp: 
// �йش����ʵ�֣������ FastDirOpen.cpp
//

class CFastDirOpenApp : public CWinApp
{
public:
	CFastDirOpenApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFastDirOpenApp theApp;
