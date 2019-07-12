// CDialogSetCustomTime.cpp: 实现文件
//

#include "stdafx.h"
#include "FileTimeChanger.h"
#include "CDialogSetCustomTime.h"
#include "afxdialogex.h"


// CDialogSetCustomTime 对话框

IMPLEMENT_DYNAMIC(CDialogSetCustomTime, CDialogEx)

CDialogSetCustomTime::CDialogSetCustomTime(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DATASET, pParent)
{
	GetLocalTime(&m_CustomTime);
}

CDialogSetCustomTime::~CDialogSetCustomTime()
{
}

void CDialogSetCustomTime::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATE, m_DatePicker);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TIME, m_TimePicker);
}


BEGIN_MESSAGE_MAP(CDialogSetCustomTime, CDialogEx)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_DATE, &CDialogSetCustomTime::OnDtnDatetimechangeDatetimepickerDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_TIME, &CDialogSetCustomTime::OnDtnDatetimechangeDatetimepickerTime)
END_MESSAGE_MAP()



void CDialogSetCustomTime::OnDtnDatetimechangeDatetimepickerDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	SYSTEMTIME t;
	m_DatePicker.GetTime(&t);
	m_CustomTime.wDay = t.wDay;
	m_CustomTime.wMonth = t.wMonth;
	m_CustomTime.wYear = t.wYear;
	*pResult = 0;
}


void CDialogSetCustomTime::OnDtnDatetimechangeDatetimepickerTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	SYSTEMTIME t;
	m_DatePicker.GetTime(&t);
	m_CustomTime.wHour = t.wHour;
	m_CustomTime.wMinute = t.wMinute;
	m_CustomTime.wSecond = t.wSecond;
	*pResult = 0;
}
