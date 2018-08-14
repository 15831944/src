#include "stdafx.h"
#include "FastDirOpen.h"
#include "FastDirOpenDlg.h"
#include "afxdialogex.h"

#include "ShowDirWindow.h"
#include "ConfigSet.h"
#include "Resource.h"


#define INI_PATH TEXT("mydeardirs")


//DirList������غ���

#define TOTAL 15.0
/*****************************************
���ܣ����ݱ�������CListCtrl�ռ��п�
*****************************************/
BOOL ChangeListTitleSize(CListCtrl *cListCtrl) {

	CRect rect;
	GetClientRect(cListCtrl->m_hWnd, &rect);
	//rect.top += 50;
	//rect.bottom -= 10;
	//rect.left += 10;
	//rect.right -= 0;

	cListCtrl->SetColumnWidth(NAME_COL, (int)(3.0 *rect.Width() / TOTAL));
	cListCtrl->SetColumnWidth(1, (int)(7.0*rect.Width() / TOTAL));
	cListCtrl->SetColumnWidth(2, (int)(2.0*rect.Width() / TOTAL));
	cListCtrl->SetColumnWidth(3, rect.Width() - (int)(12.0*rect.Width() / TOTAL));

	//m_houseListCtrl.MoveWindow(&rect);

	return TRUE;
}


/*****************************************
���ܣ���ʼ��Ŀ¼�б�
*****************************************/
void CFastDirOpenDlg::InitDirList(){

	//1. ������չ��ʽ
	m_FastDirList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//ѡ��һ�У���ʾ����
	//ps.GetExtendedStyle() ��ȡ��չ��ʽ

	//2. ����м�����
	m_FastDirList.InsertColumn(NAME_COL, TEXT("FILE NAME"));
	m_FastDirList.InsertColumn(1, TEXT("FULL PATH"));
	m_FastDirList.InsertColumn(2, TEXT("DIR STATUS"));


	m_FastDirList.InsertColumn(3, TEXT("REMARK"));

	//3. �޸��п�
	ChangeListTitleSize(&m_FastDirList);

	m_iRow = 0;

	//4. ��ȡ�����ļ�
	TCHAR *szReadData = new TCHAR[1024 * 1024];//�����ļ���СӦ�ò��ᳬ��1MB
	RtlZeroMemory(szReadData, 1024 * 1024);

	ReadFromFile(INI_PATH, szReadData);

	TCHAR szName[DEFAULT_SIZE] = {0};
	TCHAR szPath[DEFAULT_SIZE] = {0};
	TCHAR szRemark[DEFAULT_SIZE] = { 0 };
	while(_tcslen(szReadData)){
		GetItemFromData(szReadData, szName, szPath, szRemark);

		m_FastDirList.InsertItem(m_iRow, szName);
		m_FastDirList.SetItemText(m_iRow, PATH_COL, szPath);
		m_FastDirList.SetItemText(m_iRow, STATUS_COL, (PathFileExists(szPath) ? TEXT("����") : TEXT("������")));
		m_FastDirList.SetItemText(m_iRow, REMARK_COL, szRemark);

		m_iRow++;
	}

	if(szReadData){
		delete[] szReadData;
	}
	return ;
}


/*****************************************
���ܣ����item
*****************************************/
void CFastDirOpenDlg::AddOneDir(LPTSTR lpName, LPTSTR lpPath, LPTSTR lpRemark){

	//5.�ж��Ƿ��ظ����
	if(TRUE == CheckRepeatDir(lpPath)){
		::MessageBox(NULL, TEXT("�����ظ����"), TEXT("ADD ERR"), MB_ICONWARNING|MB_OK);
		return ;
	}

	//6.д��ini�ļ�
	HANDLE hWriteFile = CreateFile(INI_PATH, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hWriteFile != INVALID_HANDLE_VALUE){
		WriteItemToFile(hWriteFile, lpName, lpPath, lpRemark);
		CloseHandle(hWriteFile);
	}

	//7. ��ʾ����
	m_FastDirList.InsertItem(m_iRow, lpName);
	m_FastDirList.SetItemText(m_iRow, PATH_COL, lpPath);
	m_FastDirList.SetItemText(m_iRow, STATUS_COL, (PathFileExists(lpPath)? TEXT("����"):TEXT("������")));
	m_FastDirList.SetItemText(m_iRow, REMARK_COL, lpRemark);
	m_iRow++;
		
	
	return;
}


/*****************************************
���ܣ����˫����ʾ�ļ��н���
*****************************************/
void CFastDirOpenDlg::OnNMDBClickListFastdir(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	TCHAR szDirPath[DEFAULT_SIZE] = { 0 };
	TCHAR szDirStatus[DEFAULT_SIZE] = { 0 };

	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));

	m_FastDirList.ScreenToClient(&point);

	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;

	int nItem = m_FastDirList.SubItemHitTest(&lvinfo);
	if (nItem >= 0 && nItem < m_iRow)
	{
		m_FastDirList.GetItemText(nItem, PATH_COL, szDirPath, DEFAULT_SIZE);
		m_iCurrentItem = nItem;

		if(PathFileExists(szDirPath))
			ShowDirAlways(szDirPath);
		else{
			m_FastDirList.SetItemText(nItem, STATUS_COL, TEXT("������"));
		}
	}

	*pResult = 0;
}


/*****************************************
���ܣ��Ҽ�������ʾ�Ҽ��˵�
*****************************************/
void CFastDirOpenDlg::OnNMRClickListFastdir(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPoint pt;//�һ��˵������Ͻǵ�λ���ǻ�����Ļ����ϵ  

	CMenu m;
	
	do{
		//1. ����ǲ��ǿ���
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));

		m_FastDirList.ScreenToClient(&point);

		LVHITTESTINFO lvinfo;
		lvinfo.pt = point;
		lvinfo.flags = LVHT_ABOVE;

		int nItem = m_FastDirList.SubItemHitTest(&lvinfo);//��õ�ǰ������к�

		//2. ��ʾ�Ҽ��˵�
		GetCursorPos(&pt);//������λ��,��Ļλ��
		//ClientToScreen(&pt);//��������Ǵ�����������Ҫת��Ϊ��Ļ����

		if(!m.LoadMenu(IDR_MENU_DELETE)){//���ز˵���Դ��IDR_MENU_DELETE�ĵ�һ���˵���Ҫ����һ���˵�����TrackPopupMenu
			ThrowMes(TEXT("LoadMenu"));
			break;
		}

		if(!m.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this)){//��ʾ�˵����������������Ļ����
			ThrowMes(TEXT("TrackPopupMenu"));
			break;
		}

		m_iCurrentItem = nItem;
	}while(FALSE);

	*pResult = 0;
}


/*****************************************
���ܣ�����Ƿ��Ѿ���Ӹ�·��
*****************************************/
BOOL CFastDirOpenDlg::CheckRepeatDir(LPTSTR lpNewPath) {

	BOOL bRet = FALSE;
	TCHAR szTemp[DEFAULT_SIZE] = {0};

	for(int i = 0; i < m_iRow; i++){
		m_FastDirList.GetItemText(i, PATH_COL, szTemp, DEFAULT_SIZE);
		if(_tcsicmp(szTemp, lpNewPath) == 0){
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}


/*****************************************
���ܣ����������ļ���
*****************************************/
BOOL CFastDirOpenDlg::ClearInvaildDir() {

	BOOL bRet = TRUE;
	TCHAR szTemp[DEFAULT_SIZE] = { 0 };
	int iCount = m_iRow;

	for (int i = iCount - 1; i >= 0; i--) {//������뵹��������Ϊÿɾ��һ��item��ʣ���item�ͻ��Զ��ƶ���eg:ɾ����0����1-8�ͱ��0-7,ԭ����8�����7����ɾ��8�͵�deleteitem(7)
		m_FastDirList.GetItemText(i, PATH_COL, szTemp, DEFAULT_SIZE);
		if (!PathFileExists(szTemp)) {
			m_FastDirList.DeleteItem(i);
			m_iRow--;
		}
	}

	return bRet;
}


/*****************************************
���ܣ�ɾ������item
*****************************************/
BOOL CFastDirOpenDlg::ClearAllDir() {

	BOOL bRet = TRUE;
	TCHAR szTemp[DEFAULT_SIZE] = { 0 };
	int iCount = m_iRow;//;

	for (int i = iCount - 1; i >= 0; i--) {
		m_FastDirList.DeleteItem(i);
	}

	m_iRow = 0;

	return bRet;
}


/*****************************************
���ܣ���¼����item
*****************************************/

BOOL CFastDirOpenDlg::WriteAllItems() {

	BOOL bRet = TRUE;
	
	HANDLE hWriteFile = CreateFile(INI_PATH, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	TCHAR szName[DEFAULT_SIZE] = {0};
	TCHAR szPath[DEFAULT_SIZE] = { 0 };
	TCHAR szRemark[DEFAULT_SIZE] = { 0 };

	do{
		if(INVALID_HANDLE_VALUE == hWriteFile)
			break;

		for(int i = 0; i < m_iRow; i++){
			m_FastDirList.GetItemText(i, NAME_COL, szName, DEFAULT_SIZE);
			m_FastDirList.GetItemText(i, PATH_COL, szPath, DEFAULT_SIZE);
			m_FastDirList.GetItemText(i, REMARK_COL, szRemark, DEFAULT_SIZE);
			WriteItemToFile(hWriteFile, szName, szPath, szRemark);
		}

	} while(FALSE);

	if(hWriteFile)
		CloseHandle(hWriteFile);

	return bRet;
}


/*****************************************
���ܣ�ɾ��һ��
*****************************************/
void CFastDirOpenDlg::DeleteOneItem(){
	// TODO: �ڴ���������������	
	if (m_iCurrentItem < 0 || m_iCurrentItem >= m_iRow) {
		::MessageBox(NULL, TEXT("����ɾ�����������ڵĶ���"), TEXT("DEL ERR"), MB_ICONWARNING | MB_OK);
	}
	else {
		TCHAR szName[DEFAULT_SIZE] = {0};
		TCHAR szPath[DEFAULT_SIZE] = { 0 };
		TCHAR szRemark[DEFAULT_SIZE] = { 0 };

		m_FastDirList.GetItemText(m_iCurrentItem, NAME_COL, szName, DEFAULT_SIZE);
		m_FastDirList.GetItemText(m_iCurrentItem, PATH_COL, szPath, DEFAULT_SIZE);
		m_FastDirList.GetItemText(m_iCurrentItem, REMARK_COL, szRemark, DEFAULT_SIZE);
		DeleteAnItem(INI_PATH, szName, szPath, szRemark);
		
		m_FastDirList.DeleteItem(m_iCurrentItem);
		m_iRow--;
		m_iCurrentItem = -1;
	}
}



/*****************************************
���ܣ���õ�ǰ���ָ����У���ֵ��m_iCurrentItem
*****************************************/
void CFastDirOpenDlg::GetCurrentItemNum(){
	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));

	m_FastDirList.ScreenToClient(&point);

	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;

	m_iCurrentItem = m_FastDirList.SubItemHitTest(&lvinfo);

	return ;
}