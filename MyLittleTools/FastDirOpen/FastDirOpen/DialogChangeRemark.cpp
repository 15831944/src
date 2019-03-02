// DialogChangeRemark.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FastDirOpen.h"
#include "DialogChangeRemark.h"
#include "afxdialogex.h"


// CDialogChangeRemark �Ի���

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


// CDialogChangeRemark ��Ϣ�������


//����OnOK�������س���Ĭ��ʹ��OnOK()����������������ûس�����ʱ�����������ݾ�Ҫ�Լ������������
void CDialogChangeRemark::OnOK(){
	m_EditRemark.GetWindowText(m_szRemark, DEFAULT_SIZE);

	CDialog::OnOK();
}

void CDialogChangeRemark::OnBnClickedButtonChangeremark()
{
	OnOK();
}
