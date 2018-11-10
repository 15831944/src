// AddDirPath.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FastDirOpenDlg.h"
#include "CMyListCtrl.h"
#include "afxdialogex.h"
#include "Resource.h"

// CAddDirPath �Ի���
//CMyListCtrl::CMyListCtrl()
//	: CMyListCtrl()
//{
//}

CMyListCtrl::~CMyListCtrl()
{
}

void CMyListCtrl::DoDataExchange(CDataExchange* pDX)
{
	CListCtrl::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CMyListCtrl ��Ϣ�������
/************************************
���ܣ���ӦCMyListCtrl���ڵ��ļ���ק��Ϣ��֧�ֶ��ļ���ק,��CFastDirOpenDlg���ڵģ�
��������ק��Ϣ����Ϣ
����ֵ����
************************************/
void CMyListCtrl::OnDropFiles(HDROP hDropInfo)
{
	if (hDropInfo)
	{
		TCHAR szFilePath[DEFAULT_SIZE] = { 0 };
		int nDrag = DragQueryFile(hDropInfo, -1, NULL, 0);//��ȡһ����ק���ļ��ĸ���

		for (int i = 0; i < nDrag; i++) {
			RtlZeroMemory(szFilePath, DEFAULT_SIZE);
			DragQueryFile(hDropInfo, i, szFilePath, 1024);//��õ�һ���ļ���·��m_szFilePath������·������
														  //MessageBox(TEXT(""), szFilePath, 0);
			AddOneDir(_tcsrchr(szFilePath, TEXT('\\')) + 1, szFilePath, TEXT("NULL"));
		}
	}

	DragFinish(hDropInfo);
}