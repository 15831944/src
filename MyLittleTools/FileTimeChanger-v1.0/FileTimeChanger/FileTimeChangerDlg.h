
// FileTimeChangerDlg.h: 头文件
//

#pragma once
#include "CFileListCtrl.h"
#include "CDiskFileTree.h"

class CFileTimeChangerDlg : public CDialogEx
{
public:
	CFileTimeChangerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILETIMECHANGER_DIALOG };
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
	DECLARE_MESSAGE_MAP()

public:
	void DisableItem(int ID);
	void EnableItem(int ID);

	void DisableNoSelectTarTimeCheck();
	void ControlContractChoice(int iID, int iChangeType);
	void DisableOtherTarTimeSet(int *iWidgetArray, int iTarTimeID);
	void EnableAllTarTimeSet(int *iWidgetArray);

	void SetTarTimeType(int *iWidgetArray, int *iTarTimeID, int ID);
	void SetCTTarTimeType(int iStaticTextID);
	void SetATTarTimeType(int iStaticTextID);
	void SetMTTarTimeType(int iStaticTextID);

	void SetChangeTime(int *iWidgetArray, int iMask);

	void SetCustomTime(SYSTEMTIME* time, int id);

	void ChangeTime(LPTSTR);//
private:
	int m_ChangeTimeType; //mt at ct
	int m_CTTarTimeID;
	int m_ATTarTimeID;
	int m_MTTarTimeID;
	SYSTEMTIME m_CTCustomTime;
	SYSTEMTIME m_ATCustomTime;
	SYSTEMTIME m_MTCustomTime;

	int m_CTWidget[4];
	int m_ATWidget[4];
	int m_MTWidget[4];

	int m_TypeWidget[3];

public:
	CFileListCtrl m_FileList;
	CDiskFileTree m_DiskTree;
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedCheckTypeC();
	afx_msg void OnBnClickedCheckTypeA();
	afx_msg void OnBnClickedCheckTypeM();
	afx_msg void OnBnClickedCheckC2A();
	afx_msg void OnBnClickedCheckC2M();
	afx_msg void OnBnClickedCheckA2C();
	afx_msg void OnBnClickedCheckA2M();
	afx_msg void OnBnClickedCheckCTD();
	afx_msg void OnBnClickedCheckATD();
	afx_msg void OnBnClickedCheckM2C();
	afx_msg void OnBnClickedCheckM2A();
	afx_msg void OnBnClickedCheckMTD();
	afx_msg void OnBnClickedButtonFilelistupdate(); 
	afx_msg void OnTvnItemexpandingTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonUpdateDiskTree();
	afx_msg void OnTvnBegindragTree(NMHDR *pNMHDR, LRESULT *pResult);
};