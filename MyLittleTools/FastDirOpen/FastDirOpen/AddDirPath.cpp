// AddDirPath.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FastDirOpen.h"
#include "AddDirPath.h"
#include "afxdialogex.h"


// CAddDirPath �Ի���

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


// CAddDirPath ��Ϣ�������


/*****************************************
���ܣ�SHBrowseForFolder�ص��������ڴ����ļ�����Ի�������ʾĬ��·��
*****************************************/
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {

	switch (uMsg) {
	case BFFM_INITIALIZED:  //ѡ���ļ��жԻ����ʼ��  
							//����Ĭ��·��ΪlpData��'D:\'  
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		//��STATUSTEXT������ʾ��ǰ·��  
		::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, lpData);
		break;
	}

	return 0;
}


/*****************************************
���ܣ����·��
*****************************************/
void CAddDirPath::OnBnClickedButtonBrowser()
{
	TCHAR szFolderPath[DEFAULT_SIZE] = {0};

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BROWSEINFO sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("��ѡ��Ҫ��ӵ��ļ���");
	sInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_DONTGOBELOWDOMAIN;

	sInfo.lParam = (LPARAM)m_szDefaultDir;//�����ص������Ĳ�������Ĭ��·��
	sInfo.lpfn = (BFFCALLBACK)(BrowseCallbackProc);

	do{
		//1. �����Ի���
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
		if (lpidlBrowse == NULL)
			break;

		//2.��ȡѡ����ļ�
		if (!::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
			break;

		//3.����·��
		_tcscpy(m_szPath, szFolderPath);

		//4.�����´ε�Ĭ��·��
		TCHAR *pos = _tcsrchr(szFolderPath, TEXT('\\'));
		_tcsncpy_s(m_szDefaultDir, DEFAULT_SIZE, szFolderPath, pos - szFolderPath);

		//5.��ʾ����
		m_EditPath.SetWindowText(szFolderPath);

	}while(FALSE);

}



void CAddDirPath::OnOK(){
	m_EditRemark.GetWindowText(m_szRemark, DEFAULT_SIZE);
	CDialog::OnOK();
}
/*****************************************
���ܣ����OK
*****************************************/
void CAddDirPath::OnBnClickedButtonSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
