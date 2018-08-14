#pragma once
#include "afxwin.h"


// CAddDirPath �Ի���

class CAddDirPath : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDirPath)

public:
	CAddDirPath(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddDirPath();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowser();

private:
	TCHAR m_szDefaultDir[DEFAULT_SIZE];
	CEdit m_EditPath;
	CEdit m_EditRemark;
	
	void OnOK();

public:
	TCHAR m_szPath[DEFAULT_SIZE];
	TCHAR m_szRemark[DEFAULT_SIZE];

	afx_msg void OnBnClickedButtonSet();
};
