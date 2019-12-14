#pragma once
#include <afxwin.h>
class CMyEdit :
	public CEdit
{
protected:
	DECLARE_MESSAGE_MAP()
public:
	CMyEdit();
	~CMyEdit();
public:
	void OnDropFiles(HDROP hDropInfo);
	BOOL PreTranslateMessage(MSG* pMsg);
	bool SetTextFont(int iSize);

public:
	CEdit* mFilePathEdit;
	int mHeight;
	bool mSelectAll;
	int mLastStart;
	int mLastEnd;
};

