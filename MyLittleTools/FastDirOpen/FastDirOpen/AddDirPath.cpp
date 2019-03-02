// AddDirPath.cpp : 实现文件
//

#include "stdafx.h"
#include "FastDirOpen.h"
#include "AddDirPath.h"
#include "afxdialogex.h"


// CAddDirPath 对话框

IMPLEMENT_DYNAMIC(CAddDirPath, CDialogEx)

CAddDirPath::CAddDirPath(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ADD, pParent)
{

}

CAddDirPath::~CAddDirPath()
{
}

void CAddDirPath::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditPath);
	DDX_Control(pDX, IDC_EDIT2, m_EditRemark);
}


BEGIN_MESSAGE_MAP(CAddDirPath, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BROWSER, &CAddDirPath::OnBnClickedButtonBrowser)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CAddDirPath::OnBnClickedButtonSet)
END_MESSAGE_MAP()


// CAddDirPath 消息处理程序


/*****************************************
功能：SHBrowseForFolder回调函数，在创建文件浏览对话框是显示默认路径
*****************************************/
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {

	switch (uMsg) {
	case BFFM_INITIALIZED:  //选择文件夹对话框初始化  
							//设置默认路径为lpData即'D:\'  
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		//在STATUSTEXT区域显示当前路径  
		::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, lpData);
		break;
	}

	return 0;
}


/*****************************************
功能：浏览路径
*****************************************/
void CAddDirPath::OnBnClickedButtonBrowser()
{
	TCHAR szFolderPath[DEFAULT_SIZE] = {0};

	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("请选择要添加的文件夹");
	sInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_DONTGOBELOWDOMAIN;

	sInfo.lParam = (LPARAM)m_szDefaultDir;//传给回调函数的参数——默认路径
	sInfo.lpfn = (BFFCALLBACK)(BrowseCallbackProc);

	do{
		//1. 创建对话框
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
		if (lpidlBrowse == NULL)
			break;

		//2.获取选择的文件
		if (!::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
			break;

		//3.赋给路径
		_tcscpy(m_szPath, szFolderPath);

		//4.设置下次的默认路径
		TCHAR *pos = _tcsrchr(szFolderPath, TEXT('\\'));
		_tcsncpy_s(m_szDefaultDir, DEFAULT_SIZE, szFolderPath, pos - szFolderPath);

		//5.显示出来
		m_EditPath.SetWindowText(szFolderPath);

	}while(FALSE);

}



void CAddDirPath::OnOK(){
	m_EditRemark.GetWindowText(m_szRemark, DEFAULT_SIZE);
	CDialog::OnOK();
}
/*****************************************
功能：点击OK
*****************************************/
void CAddDirPath::OnBnClickedButtonSet()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
