#pragma once
#include "afxwin.h"


// CDialogChangeRemark �Ի���

class CDialogChangeRemark : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogChangeRemark)

public:
	CDialogChangeRemark(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogChangeRemark();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EDITREMARK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	TCHAR m_szRemark[DEFAULT_SIZE] = {0};
	CEdit m_EditRemark;
	afx_msg void OnBnClickedButtonChangeremark();
	void CDialogChangeRemark::OnOK();
};
