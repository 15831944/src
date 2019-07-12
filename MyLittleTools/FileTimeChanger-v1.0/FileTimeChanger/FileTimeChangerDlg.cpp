
// FileTimeChangerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "FileTimeChanger.h"
#include "FileTimeChangerDlg.h"
#include "afxdialogex.h"
#include "CDialogSetCustomTime.h"
#include "DealFileTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFileTimeChangerDlg 对话框

CFileTimeChangerDlg::CFileTimeChangerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILETIMECHANGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_CTWidget[0] = IDC_CHECK_C2A;
	m_CTWidget[1] = IDC_CHECK_C2M;
	m_CTWidget[2] = IDC_CHECK_CTD;
	m_CTWidget[3] = IDC_STATIC_CT;

	m_ATWidget[0] = IDC_CHECK_A2C;
	m_ATWidget[1] = IDC_CHECK_A2M;
	m_ATWidget[2] = IDC_CHECK_ATD;
	m_ATWidget[3] = IDC_STATIC_AT;

	m_MTWidget[0] = IDC_CHECK_M2C;
	m_MTWidget[1] = IDC_CHECK_M2A;
	m_MTWidget[2] = IDC_CHECK_MTD;
	m_MTWidget[3] = IDC_STATIC_MT;

	m_TypeWidget[0] = IDC_CHECK_TYPE1;
	m_TypeWidget[1] = IDC_CHECK_TYPE2;
	m_TypeWidget[2] = IDC_CHECK_TYPE3;

	m_ChangeTimeType = 0;

	m_CTTarTimeID = -1;
	m_ATTarTimeID = -1;
	m_MTTarTimeID = -1;
}

void CFileTimeChangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE, m_FileList);
	DDX_Control(pDX, IDC_TREE, m_DiskTree);
}

BEGIN_MESSAGE_MAP(CFileTimeChangerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CFileTimeChangerDlg::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CFileTimeChangerDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_CHECK_TYPE1, &CFileTimeChangerDlg::OnBnClickedCheckTypeC)
	ON_BN_CLICKED(IDC_CHECK_TYPE2, &CFileTimeChangerDlg::OnBnClickedCheckTypeA)
	ON_BN_CLICKED(IDC_CHECK_TYPE3, &CFileTimeChangerDlg::OnBnClickedCheckTypeM)
	ON_BN_CLICKED(IDC_CHECK_C2A, &CFileTimeChangerDlg::OnBnClickedCheckC2A)
	ON_BN_CLICKED(IDC_CHECK_C2M, &CFileTimeChangerDlg::OnBnClickedCheckC2M)
	ON_BN_CLICKED(IDC_CHECK_A2C, &CFileTimeChangerDlg::OnBnClickedCheckA2C)
	ON_BN_CLICKED(IDC_CHECK_A2M, &CFileTimeChangerDlg::OnBnClickedCheckA2M)
	ON_BN_CLICKED(IDC_CHECK_CTD, &CFileTimeChangerDlg::OnBnClickedCheckCTD)
	ON_BN_CLICKED(IDC_CHECK_ATD, &CFileTimeChangerDlg::OnBnClickedCheckATD)
	ON_BN_CLICKED(IDC_CHECK_M2C, &CFileTimeChangerDlg::OnBnClickedCheckM2C)
	ON_BN_CLICKED(IDC_CHECK_M2A, &CFileTimeChangerDlg::OnBnClickedCheckM2A)
	ON_BN_CLICKED(IDC_CHECK_MTD, &CFileTimeChangerDlg::OnBnClickedCheckMTD)

	ON_BN_CLICKED(IDC_BUTTON_FILELISTUPDATE, &CFileTimeChangerDlg::OnBnClickedButtonFilelistupdate)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE, &CFileTimeChangerDlg::OnTvnItemexpandingTree)
	ON_BN_CLICKED(IDC_BUTTON1, &CFileTimeChangerDlg::OnBnClickedButtonUpdateDiskTree)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE, &CFileTimeChangerDlg::OnTvnBegindragTree)
END_MESSAGE_MAP()


// CFileTimeChangerDlg 消息处理程序

BOOL CFileTimeChangerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//1.
	DisableNoSelectTarTimeCheck();
	
	//2.
	m_FileList.InitFileListCtrlTitle();
	m_FileList.InitFileListColWidth();

	//3.Tree 
	m_DiskTree.InitTreeRoot();
	m_DiskTree.InitOpenDir();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFileTimeChangerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFileTimeChangerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFileTimeChangerDlg::SetCustomTime(SYSTEMTIME* time,int id) {
	CWnd* pWnd = GetFocus();
	if (((CButton *)pWnd)->GetCheck()) {
		CDialogSetCustomTime dialogSetTime;

		int iRes = dialogSetTime.DoModal();
		if (IDOK == iRes) {

			SYSTEMTIME cCustomTime = dialogSetTime.m_CustomTime;
			TCHAR tmp[1024] = { 0 };
			::wsprintf(tmp, TEXT("CusTime: %d-%d-%d %d:%d:%d"), cCustomTime.wYear, cCustomTime.wMonth, cCustomTime.wDay, cCustomTime.wHour, cCustomTime.wMinute, cCustomTime.wSecond);
			SetDlgItemText(id, tmp);
			(*time) = cCustomTime;
		}
	}
	else
		(*time) = {0,0,0,0,0,0,0,0};
}

void CFileTimeChangerDlg::ChangeTime(LPTSTR szFilePath) {
	FILE_TIMES pft;
	GetFileTimes(szFilePath, &pft);

	if ((m_ChangeTimeType & 0xf)) {
		//ct
		switch (m_CTTarTimeID)
		{
		case IDC_CHECK_C2A:
			pft.ct = pft.at;
			break;
		case IDC_CHECK_C2M:
			pft.ct = pft.mt;
			break;
		case IDC_CHECK_CTD:
			pft.ct = m_CTCustomTime;
			break;
		}
	}

	if ((m_ChangeTimeType & 0xf0)) {
		//at
		switch (m_ATTarTimeID)
		{
		case IDC_CHECK_A2C:
			pft.at = pft.ct;
			break;
		case IDC_CHECK_A2M:
			pft.at = pft.mt;
			break;
		case IDC_CHECK_ATD:
			pft.at = m_ATCustomTime;
			break;
		}
	}

	if ((m_ChangeTimeType & 0xf00)) {
		//mt
		switch (m_MTTarTimeID)
		{
		case IDC_CHECK_M2C:
			pft.mt = pft.ct;
			break;
		case IDC_CHECK_M2A:
			pft.mt = pft.at;
			break;
		case IDC_CHECK_MTD:
			pft.mt = m_MTCustomTime;
			break;
		}
	}
	int re1 = memcmp(&pft.ct, &pft.at, sizeof(SYSTEMTIME));
	int re2 = memcmp(&pft.at, &pft.mt, sizeof(SYSTEMTIME));
	if (re1 <= 0 && re2 <= 0)
		ChangeFileTimes(szFilePath, &pft);
	else {
		if(IDYES == MesBox(this->m_hWnd, MB_ICONWARNING | MB_YESNO, TEXT("WARNING!"), TEXT("The FileTime may not Reasonable.\nFilePath:%s\nContinue?"), szFilePath))
			ChangeFileTimes(szFilePath, &pft);
	}
}

//test now
void CFileTimeChangerDlg::OnBnClickedButtonChange()
{
	if (m_ChangeTimeType) {
		vector<LPTSTR> vecFileList;
		m_FileList.GetFileList(&vecFileList);
		vector<LPTSTR>::iterator it;
		while (FALSE == vecFileList.empty()) {
			it = vecFileList.end() - 1;
			ChangeTime(*it);
			free((*it));
			vecFileList.pop_back();
		}
		m_FileList.UpdateFileList();
	}
}

void CFileTimeChangerDlg::OnBnClickedButtonClear(){	
	CButton *pWnd;
	for (int i = 0; i < 3; i++) {
		pWnd = (CButton *)GetDlgItem(m_TypeWidget[i]);
		pWnd->SetCheck(0);
	}

	m_ChangeTimeType = 0;
	DisableNoSelectTarTimeCheck();

	SetDlgItemText(IDC_STATIC_CT, TEXT("CusTime:"));
	SetDlgItemText(IDC_STATIC_AT, TEXT("CusTime:"));
	SetDlgItemText(IDC_STATIC_MT, TEXT("CusTime:"));

	m_FileList.ClearFileList();
}


//Set Change Time Type
//---------------------------------------------------------
void CFileTimeChangerDlg::OnBnClickedCheckTypeC()
{
	SetChangeTime(m_CTWidget, CTMask);
}

void CFileTimeChangerDlg::OnBnClickedCheckTypeA()
{
	SetChangeTime(m_ATWidget, ATMask);
}

void CFileTimeChangerDlg::OnBnClickedCheckTypeM()
{
	SetChangeTime(m_MTWidget, MTMask);
}


//Set Target Time Type
//----------------------------------------------------------------------------
void CFileTimeChangerDlg::OnBnClickedCheckC2A()
{
	int iChangeAtime = m_ChangeTimeType & ATMask;
	ControlContractChoice(IDC_CHECK_A2C, iChangeAtime);
	SetCTTarTimeType(IDC_STATIC_CT);
}
void CFileTimeChangerDlg::OnBnClickedCheckC2M()
{
	int iChangeMtime = m_ChangeTimeType & MTMask;
	ControlContractChoice(IDC_CHECK_M2C, iChangeMtime);
	SetCTTarTimeType(IDC_STATIC_CT);
}
void CFileTimeChangerDlg::OnBnClickedCheckCTD()
{
	SetCTTarTimeType(IDC_STATIC_CT);
	SetCustomTime(&m_CTCustomTime, IDC_STATIC_CT);
}


void CFileTimeChangerDlg::OnBnClickedCheckA2C()
{
	int iChangeCtime = m_ChangeTimeType & CTMask;
	ControlContractChoice(IDC_CHECK_C2A, iChangeCtime);
	SetATTarTimeType(IDC_STATIC_AT);
}
void CFileTimeChangerDlg::OnBnClickedCheckA2M()
{
	int iChangeMtime = m_ChangeTimeType & MTMask;
	ControlContractChoice(IDC_CHECK_M2A, iChangeMtime);
	SetATTarTimeType(IDC_STATIC_AT);
}
void CFileTimeChangerDlg::OnBnClickedCheckATD()
{
	SetATTarTimeType(IDC_STATIC_AT);
	SetCustomTime(&m_ATCustomTime, IDC_STATIC_AT);
}


void CFileTimeChangerDlg::OnBnClickedCheckM2C()
{
	int iChangeCtime = m_ChangeTimeType & CTMask;
	ControlContractChoice(IDC_CHECK_C2M, iChangeCtime);
	SetMTTarTimeType(IDC_STATIC_MT);
}
void CFileTimeChangerDlg::OnBnClickedCheckM2A()
{
	int iChangeAtime = m_ChangeTimeType & ATMask;
	ControlContractChoice(IDC_CHECK_A2M, iChangeAtime);
	SetMTTarTimeType(IDC_STATIC_MT);
}
void CFileTimeChangerDlg::OnBnClickedCheckMTD()
{
	SetMTTarTimeType(IDC_STATIC_MT);
	SetCustomTime(&m_MTCustomTime, IDC_STATIC_MT);
}

void CFileTimeChangerDlg::OnBnClickedButtonFilelistupdate()
{
	m_FileList.UpdateFileList();
}

void CFileTimeChangerDlg::OnTvnItemexpandingTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFullPath[DSIZE] = { 0 };
	m_DiskTree.SelectItem(pNMTreeView->itemNew.hItem);

	m_DiskTree.GetFullPath(pNMTreeView->itemNew.hItem, szFullPath);
	if (!PathIsDirectory(szFullPath))//文件当然没有子节点
		return;

	if (0 != (pNMTreeView->itemNew.state & TVIS_EXPANDED)) {
		m_DiskTree.DelOpenDirInReg(szFullPath);
		return;
	}
	else {
		m_DiskTree.AddOpenDirToReg(szFullPath);
	}

	if (NULL != m_DiskTree.GetChildItem(pNMTreeView->itemNew.hItem)) {//已经初始化过了，不需要再insert了
		return;
	}
	else {
		m_DiskTree.InsertSingleFileList(pNMTreeView->itemNew.hItem, szFullPath, szFullPath);
	}

	*pResult = 0;
}


void CFileTimeChangerDlg::OnBnClickedButtonUpdateDiskTree()
{
	// TODO: 在此添加控件通知处理程序代码
	m_DiskTree.UpDateWholeTree();
}


void CFileTimeChangerDlg::OnTvnBegindragTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szPath[DSIZE] = { 0 };
	m_DiskTree.GetFullPath(pNMTreeView->itemNew.hItem, szPath);
	if (FALSE == PathIsDirectory(szPath)) {
		if (FALSE == m_FileList.IsRepeat(szPath))
			m_FileList.InsertOneFile(szPath);
		else {
			MesBox(this->m_hWnd, MB_ICONERROR | MB_OK, TEXT("ERROR"), TEXT("A OU,<%s> is already in list."), szPath);
		}
	}
	else {
		MesBox(this->m_hWnd, MB_ICONERROR | MB_OK, TEXT("ERROR"), TEXT("A OU,<%s> is a directory.\nWe don't support it yet."), szPath);
	}
	*pResult = 0;
}
