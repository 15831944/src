#pragma once
#include "afxcmn.h"

// CMyListCtrl

class CMyListCtrl :	public CListCtrl
{
public:
	int m_iRow = 0;
	int m_iCurrentItem = -1;
	void AddOneDir(LPTSTR lpName, LPTSTR lpPath, LPTSTR lpRemark);
	void InitDirList();

	//CMyListCtrl();   // ��׼���캯��
	virtual ~CMyListCtrl();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void CMyListCtrl::OnDropFiles(HDROP hDropInfo);

	BOOL CheckRepeatDir(LPTSTR lpNewPath);
};
