
// FastDirOpenDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "CMyListCtrl.h"

#define WM_MY_SHOWTASK (WM_USER + 100) //[最小化]1.定义一个消息

// CFastDirOpenDlg 对话框
class CFastDirOpenDlg : public CDialogEx
{
// 构造
public:
	CFastDirOpenDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FASTDIROPEN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg LRESULT OnMyShowTask(WPARAM wParam, LPARAM lParam);//[最小化]2. 定义一个消息处理函数,对函数的返回值和参数都有要求
	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------
private:
	TCHAR m_szDefaultDir[DEFAULT_SIZE] = TEXT("c:\\users\\chris\\desktop");
	
	//CListCtrl与Clientwindow的边距
	float m_left = 0, m_right = 0, m_top = 0, m_bottom = 0;

	//当前点击的remark的行号
	int m_iCurRemark = -1;

	NOTIFYICONDATA m_nid;//[最小化]5.添加一个函数需要的变量

	BOOL CheckRepeatDir(LPTSTR lpNewPath);
	BOOL ClearInvaildDir();
	BOOL ClearAllDir();
	BOOL WriteAllItems();
	void DeleteOneItem();
	void OnSize(UINT nType, int cx, int cy);
	void ReSize(void);
	void GetCurrentItemNum();
	void OnTimer(UINT_PTR);
	void OnGetEidtRemark(int nItem);

	BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	CMyListCtrl m_FastDirList;
	int m_iCurrentCol;//排序时点击的列号

	afx_msg void OnNMDBClickListFastdir(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListFastdir(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRCItemDelete();
	afx_msg void OnRmenuAdd();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonClearinvalid();
	afx_msg void OnBnClickedButtonClearall();
	afx_msg void OnRclickRemark();
	afx_msg void OnLvnColumnclickListFastdir(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListFastdir(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void CFastDirOpenDlg::OnDropFiles(HDROP hDropInfo);

private:
	CEdit m_CEditRemark;
public:
	afx_msg void OnEnKillfocusEditRemark();
};