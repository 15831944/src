// AddDirPath.cpp : 实现文件
//

#include "stdafx.h"
#include "FastDirOpenDlg.h"
#include "CMyListCtrl.h"
#include "afxdialogex.h"
#include "Resource.h"

// CAddDirPath 对话框
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


// CMyListCtrl 消息处理程序
/************************************
功能：响应CMyListCtrl窗口的文件拖拽消息，支持多文件拖拽,（CFastDirOpenDlg窗口的）
参数：拖拽消息的信息
返回值：无
************************************/
void CMyListCtrl::OnDropFiles(HDROP hDropInfo)
{
	if (hDropInfo)
	{
		TCHAR szFilePath[DEFAULT_SIZE] = { 0 };
		int nDrag = DragQueryFile(hDropInfo, -1, NULL, 0);//获取一共拖拽的文件的个数

		for (int i = 0; i < nDrag; i++) {
			RtlZeroMemory(szFilePath, DEFAULT_SIZE);
			DragQueryFile(hDropInfo, i, szFilePath, 1024);//获得第一个文件的路径m_szFilePath，返回路径长度
														  //MessageBox(TEXT(""), szFilePath, 0);
			AddOneDir(_tcsrchr(szFilePath, TEXT('\\')) + 1, szFilePath, TEXT("NULL"));
		}
	}

	DragFinish(hDropInfo);
}