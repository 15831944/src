#pragma once


// CDialogSetCustomTime 对话框

class CDialogSetCustomTime : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSetCustomTime)

public:
	CDialogSetCustomTime(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogSetCustomTime();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DATASET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CDateTimeCtrl m_DatePicker;
	CDateTimeCtrl m_TimePicker;
	SYSTEMTIME m_CustomTime;
	afx_msg void OnDtnDatetimechangeDatetimepickerDate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeDatetimepickerTime(NMHDR *pNMHDR, LRESULT *pResult);
};
