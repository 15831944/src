#pragma once

#include "Crypt.h"
#include "CMyEdit.h"
#define FILE_HEAD L"i am chrishuppor"

// CEasyTreeHoleDlg 对话框
class CEasyTreeHoleDlg : public CDialogEx
{
// 构造
public:
	CEasyTreeHoleDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EASYTREEHOLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMyEdit mFilePath;
	CMyEdit mAESKey;
	CMyEdit mData;

public:
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedButtonOpen();
	void OnDropFiles(HDROP hDropInfo);
	int SaveFile(wchar_t *wszFilePath, unsigned char *aesKey, byte *byteInData, int iInSize);
	
	void PrintUnlockData();
	int ReadFromFile(wchar_t *wszFilePath, unsigned char *aesKey, byte **byteOut);
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCheckPwShow();
	afx_msg void OnBnClickedButtonFilelist();
};

