#include "stdafx.h"
#include "CFileListCtrl.h"
#include "DealFileTime.h"

BEGIN_MESSAGE_MAP(CFileListCtrl, CListCtrl)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

CFileListCtrl::CFileListCtrl()
{
}

CFileListCtrl::~CFileListCtrl()
{
}

enum COL_TITLE { PATH, CTIME, ATIME, MTIME };

void CFileListCtrl::InitFileListCtrlTitle() {
	this->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	this->InsertColumn(PATH, TEXT("FilePath"));
	this->InsertColumn(CTIME, TEXT("CreateTime"));
	this->InsertColumn(ATIME, TEXT("AccessTime"));
	this->InsertColumn(MTIME, TEXT("ModifyTime"));
}

#define TOTAL 15.0
BOOL CFileListCtrl::InitFileListColWidth() {

	//1. 首先获取整个list的宽度
	CRect rect;
	this->GetClientRect(&rect);

	//2. 按照比例分配宽度
	this->SetColumnWidth(PATH, (3.0 / TOTAL * rect.Width()));
	this->SetColumnWidth(CTIME, (4.0 / TOTAL * rect.Width()));
	this->SetColumnWidth(ATIME, (4.0 / TOTAL * rect.Width()));
	this->SetColumnWidth(MTIME, rect.Width() - (11.0 / TOTAL * rect.Width()));
	return TRUE;
}

void CFileListCtrl::InsertOneItem(FILE_LIST_ITEM fli) {
	int iNewLine = this->GetItemCount();
	this->InsertItem(iNewLine, fli.lpFilePath);

	TCHAR tmp[1024] = { 0 };
	RtlZeroMemory(tmp, 1024);
	::wsprintf(tmp, TEXT("%d-%d-%d %d:%d:%d"), fli.pft->ct.wYear, fli.pft->ct.wMonth, fli.pft->ct.wDay, fli.pft->ct.wHour, fli.pft->ct.wMinute, fli.pft->ct.wSecond);
	this->SetItemText(iNewLine, CTIME, tmp);

	RtlZeroMemory(tmp, 1024);
	::wsprintf(tmp, TEXT("%d-%d-%d %d:%d:%d"), fli.pft->at.wYear, fli.pft->at.wMonth, fli.pft->at.wDay, fli.pft->at.wHour, fli.pft->at.wMinute, fli.pft->at.wSecond);
	this->SetItemText(iNewLine, ATIME, tmp);

	RtlZeroMemory(tmp, 1024);
	::wsprintf(tmp, TEXT("%d-%d-%d %d:%d:%d"), fli.pft->mt.wYear, fli.pft->mt.wMonth, fli.pft->mt.wDay, fli.pft->mt.wHour, fli.pft->mt.wMinute, fli.pft->mt.wSecond);
	this->SetItemText(iNewLine, MTIME, tmp);
}

void CFileListCtrl::DeleteOneItem(int iPos) {
	if (iPos < 0 || iPos >= this->GetItemCount()) {
		return;
	}
	else {
		this->DeleteItem(iPos);
	}
}

void CFileListCtrl::InsertOneFile(LPTSTR szFilePath) {
	if (!PathFileExists(szFilePath)) {
		MesBox(this->m_hWnd, TEXT("ERROR"), TEXT("! No such file.\nFile Path: %s."), szFilePath);
		return;
	}

	FILE_TIMES* pft = new FILE_TIMES;
	if(pft == NULL){
		MesBox(this->m_hWnd, TEXT("ERROR"), TEXT("STH Wrong When malloc FILE_TIMES.\nErr Code : %d."), GetLastError());
		return;
	}

	FILE_LIST_ITEM fli;
	fli.lpFilePath = szFilePath;
	GetFileTimes(szFilePath, pft);
	fli.pft = pft;
	InsertOneItem(fli);
}

void CFileListCtrl::GetFileList(vector<LPTSTR> *vecFileList) {
	int iTotal = this->GetItemCount();
	for (int i = iTotal - 1; i >= 0; i--) {
		CString csTmp = this->GetItemText(i, PATH);
		int iLen = csTmp.GetLength();
		LPTSTR tmp = (LPTSTR)malloc(iLen * sizeof(TCHAR));
		RtlZeroMemory(tmp, iLen * sizeof(TCHAR));
		lstrcpy(tmp, csTmp.GetString());

		(*vecFileList).push_back(tmp);
	}
}

void CFileListCtrl::ClearFileList() {
	this->DeleteAllItems();
}

void CFileListCtrl::UpdateFileList() {
	vector<LPTSTR> vecFileList;
	GetFileList(&vecFileList);
	ClearFileList();
	vector<LPTSTR>::iterator it;
	while (FALSE == vecFileList.empty()) {
		it = vecFileList.end() - 1;
		InsertOneFile(*it);
		free((*it));
		vecFileList.pop_back();
	}
}

void CFileListCtrl::OnDropFiles(HDROP hDropInfo)
{
	if (hDropInfo)//拖拽消息指针
	{
		TCHAR szFilePath[DSIZE] = { 0 };
		int nDrag = DragQueryFile(hDropInfo, -1, NULL, 0);

		for (int i = 0; i < nDrag; i++) {
			RtlZeroMemory(szFilePath, DSIZE);
			DragQueryFile(hDropInfo, i, szFilePath, DSIZE);
			if(FALSE == PathIsDirectory(szFilePath))
				InsertOneFile(szFilePath);
			else {
				MesBox(this->m_hWnd, TEXT("ERROR"), TEXT("A OU,<%s> is a directory.\nWe don't support it yet."), szFilePath);
			}
		}
	}

	DragFinish(hDropInfo);
}

//响应del按键
BOOL CFileListCtrl::PreTranslateMessage(MSG* pMsg)
{
	int iFocusItem = 0;
	//if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_ESCAPE)     return   TRUE; //为了退出方便，我改主意了，不屏蔽ESC了
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DELETE) {
		iFocusItem = this->GetSelectionMark();
		DeleteOneItem(iFocusItem);
	}
	else
		return CListCtrl::PreTranslateMessage(pMsg);

	return TRUE;
}