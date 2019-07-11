
#include "stdafx.h"
#include "FileTimeChanger.h"
#include "FileTimeChangerDlg.h"

void CFileTimeChangerDlg::DisableItem(int ID) {
	CWnd * pWnd = GetDlgItem(ID);
	pWnd->EnableWindow(FALSE);
}

void CFileTimeChangerDlg::EnableItem(int ID) {
	CWnd * pWnd = GetDlgItem(ID);
	pWnd->EnableWindow(TRUE);
}