#pragma once
#include "afxwin.h"


// CAddDirPath 对话框

class CAddDirPath : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDirPath)

public:
	CAddDirPath(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddDirPath();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
