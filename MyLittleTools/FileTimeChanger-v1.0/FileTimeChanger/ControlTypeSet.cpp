
#include "stdafx.h"
#include "FileTimeChanger.h"
#include "FileTimeChangerDlg.h"


void CFileTimeChangerDlg::DisableNoSelectTarTimeCheck() {
	CWnd *pWnd;
	if ((m_ChangeTimeType & 0xf) == 0) {
		for (int i = 0; i < 4; i++) {
			pWnd = GetDlgItem(m_CTWidget[i]);
			((CButton*)pWnd)->SetCheck(0);
			pWnd->EnableWindow(FALSE);
		}
	}
	if ((m_ChangeTimeType & 0xf0) == 0) {
		for (int i = 0; i < 4; i++) {
			pWnd = GetDlgItem(m_ATWidget[i]);
			((CButton*)pWnd)->SetCheck(0);
			pWnd->EnableWindow(FALSE);
		}
	}
	if ((m_ChangeTimeType & 0xf00) == 0) {
		for (int i = 0; i < 4; i++) {
			pWnd = GetDlgItem(m_MTWidget[i]);
			((CButton*)pWnd)->SetCheck(0);
			pWnd->EnableWindow(FALSE);
		}
	}
}


void CFileTimeChangerDlg::ControlContractChoice(int iID, int iTypeSelected) {
	if (iTypeSelected == 0)
		return;

	CWnd *pWnd = GetFocus();
	if (((CButton *)pWnd)->GetCheck()) {
		DisableItem(iID);
	}
	else{
		EnableItem(iID);
	}
}

void CFileTimeChangerDlg::DisableOtherTarTimeSet(int *iWidgetArray, int iTarTimeID) {
	CWnd *pWnd;
	for (int i = 0; i < 3; i++) {
		if (iWidgetArray[i] == iTarTimeID)
			continue;
		DisableItem(iWidgetArray[i]);
	}
}

void CFileTimeChangerDlg::EnableAllTarTimeSet(int *iWidgetArray) {
	CWnd *pWnd;
	for (int i = 0; i < 4; i++) {
		EnableItem(iWidgetArray[i]);
	}
}

void CFileTimeChangerDlg::SetChangeTime(int *iWidgetArray, int iMask) {
	CWnd *pWnd = GetFocus();
	if (((CButton *)pWnd)->GetCheck()) {
		m_ChangeTimeType = (m_ChangeTimeType | iMask);
		EnableAllTarTimeSet(iWidgetArray);
	}
	else {
		m_ChangeTimeType = (m_ChangeTimeType & (~iMask));
		DisableNoSelectTarTimeCheck();
	}
}

void CFileTimeChangerDlg::SetTarTimeType(int *iWidgetArray, int *iTarTimeID, int iStaticTextID) {
	CWnd* pWnd = GetFocus();
	if (((CButton *)pWnd)->GetCheck()) {
		(*iTarTimeID) = pWnd->GetDlgCtrlID();
		DisableOtherTarTimeSet(iWidgetArray, *iTarTimeID);
	}
	else {
		(*iTarTimeID) = -1;
		EnableAllTarTimeSet(iWidgetArray);
		SetDlgItemText(iStaticTextID, TEXT("CusTime:"));
	}
}

void CFileTimeChangerDlg::SetCTTarTimeType(int iStaticTextID) {
	SetTarTimeType(m_CTWidget, &m_CTTarTimeID, iStaticTextID);
}
void CFileTimeChangerDlg::SetATTarTimeType(int iStaticTextID) {
	SetTarTimeType(m_ATWidget, &m_ATTarTimeID, iStaticTextID);
}
void CFileTimeChangerDlg::SetMTTarTimeType(int iStaticTextID) {
	SetTarTimeType(m_MTWidget, &m_MTTarTimeID, iStaticTextID);
}
