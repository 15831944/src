
// FastDirOpenDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "CMyListCtrl.h"

#define WM_MY_SHOWTASK (WM_USER + 100) //[��С��]1.����һ����Ϣ

// CFastDirOpenDlg �Ի���
class CFastDirOpenDlg : public CDialogEx
{
// ����
public:
	CFastDirOpenDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FASTDIROPEN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg LRESULT OnMyShowTask(WPARAM wParam, LPARAM lParam);//[��С��]2. ����һ����Ϣ������,�Ժ����ķ���ֵ�Ͳ�������Ҫ��
	DECLARE_MESSAGE_MAP()
	//-------------------------------------------------------------
private:
	TCHAR m_szDefaultDir[DEFAULT_SIZE] = TEXT("c:\\users\\chris\\desktop");
	
	//CListCtrl��Clientwindow�ı߾�
	float m_left = 0, m_right = 0, m_top = 0, m_bottom = 0;

	//��ǰ�����remark���к�
	int m_iCurRemark = -1;

	NOTIFYICONDATA m_nid;//[��С��]5.���һ��������Ҫ�ı���

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
	int m_iCurrentCol;//����ʱ������к�

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