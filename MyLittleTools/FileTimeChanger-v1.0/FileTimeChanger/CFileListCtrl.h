#pragma once
#include <afxcmn.h>
#include <vector>
using namespace std;

class CFileListCtrl :
	public CListCtrl
{
protected:
	DECLARE_MESSAGE_MAP()

public:
	CFileListCtrl();
	~CFileListCtrl();

	void InitFileListCtrlTitle();
	BOOL InitFileListColWidth();

	void GetFileList(vector<LPTSTR> *vecFileList);
	void ClearFileList();
	void UpdateFileList();

	BOOL IsRepeat(LPTSTR szPath);

	void InsertOneItem(FILE_LIST_ITEM fli);
	void InsertOneFile(LPTSTR szFilePath);
	void DelSelectedItem();

	void OnDropFiles(HDROP hDropInfo);

	BOOL PreTranslateMessage(MSG* pMsg);
};

