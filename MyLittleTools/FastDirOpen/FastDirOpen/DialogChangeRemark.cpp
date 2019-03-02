// DialogChangeRemark.cpp : 实现文件
//

#include "stdafx.h"
#include "FastDirOpen.h"
#include "DialogChangeRemark.h"
#include "afxdialogex.h"


// CDialogChangeRemark 对话框

IMPLEMENT_DYNAMIC(CDialogChangeRemark, CDialogEx)

CDialogChangeRemark::CDialogChangeRemark(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_EDITREMARK, pParent)
{

}

CDialogChangeRemark::~CDialogChangeRemark()
{
}

void CDialogChangeRemark::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CHANEGREMARK, m_EditRemark);
}


BEGIN_MESSAGE_MAP(CDialogChangeRemark, CDialogEx)
	ON_BN_CLICKED(IDC_EDIT_CHANEGREMARK, &CDialogChangeRemark::OnBnClickedButtonChangeremark)
END_MESSAGE_MAP()


// CDialogChangeRemark 消息处理程序


//重载OnOK函数，回车键默认使用OnOK()函数，所以如果想让回车返回时获得输入框内容就要自己重载这个函数
void CDialogChangeRemark::OnOK(){
	m_EditRemark.GetWindowText(m_szRemark, DEFAULT_SIZE);

	CDialog::OnOK();
}

void CDialogChangeRemark::OnBnClickedButtonChangeremark()
{
	OnOK();
}
