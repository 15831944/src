#pragma once
#include "afxwin.h"


// CDialogChangeRemark 对话框

class CDialogChangeRemark : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogChangeRemark)

public:
	CDialogChangeRemark(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogChangeRemark();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EDITREMARK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	TCHAR m_szRemark[DEFAULT_SIZE] = {0};
	CEdit m_EditRemark;
	afx_msg void OnBnClickedButtonChangeremark();
	void CDialogChangeRemark::OnOK();
};
