#include "stdafx.h"
#include "FastDirOpen.h"
#include "FastDirOpenDlg.h"
#include "afxdialogex.h"

#include "ShowDirWindow.h"
#include "ConfigSet.h"
#include "Resource.h"


#define INI_PATH TEXT("mydeardirs")


//DirList控制相关函数

#define TOTAL 15.0
/*****************************************
功能：根据比例调整CListCtrl空间列宽
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
功能：初始化目录列表
*****************************************/
void CMyListCtrl::InitDirList(){

	//1. 设置扩展样式
	this->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//选中一行，显示网格
	//ps.GetExtendedStyle() 获取扩展样式

	//2. 添加列及标题
	this->InsertColumn(NAME_COL, TEXT("FILE NAME"));
	this->InsertColumn(1, TEXT("FULL PATH"));
	this->InsertColumn(2, TEXT("DIR STATUS"));


	this->InsertColumn(3, TEXT("REMARK"));

	//3. 修改列宽
	ChangeListTitleSize(this);

	m_iRow = 0;

	//4. 读取配置文件
	TCHAR *szReadData = new TCHAR[1024 * 1024];//配置文件大小应该不会超过1MB
	RtlZeroMemory(szReadData, 1024 * 1024);

	ReadFromFile(INI_PATH, szReadData);

	TCHAR szName[DEFAULT_SIZE] = {0};
	TCHAR szPath[DEFAULT_SIZE] = {0};
	TCHAR szRemark[DEFAULT_SIZE] = { 0 };
	while(_tcslen(szReadData)){
		GetItemFromData(szReadData, szName, szPath, szRemark);

		this->InsertItem(m_iRow, szName);
		this->SetItemText(m_iRow, PATH_COL, szPath);
		this->SetItemText(m_iRow, STATUS_COL, (PathFileExists(szPath) ? TEXT("可用") : TEXT("不可用")));
		this->SetItemText(m_iRow, REMARK_COL, szRemark);

		m_iRow++;
	}

	if(szReadData){
		delete[] szReadData;
	}
	return ;
}


/*****************************************
功能：添加item
*****************************************/
void CMyListCtrl::AddOneDir(LPTSTR lpName, LPTSTR lpPath, LPTSTR lpRemark){

	//5.判断是否重复添加
	if(TRUE == CheckRepeatDir(lpPath)){
		::MessageBox(NULL, TEXT("不能重复添加"), TEXT("ADD ERR"), MB_ICONWARNING|MB_OK);
		return ;
	}

	//6.写入ini文件
	HANDLE hWriteFile = CreateFile(INI_PATH, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hWriteFile != INVALID_HANDLE_VALUE){
		WriteItemToFile(hWriteFile, lpName, lpPath, lpRemark);
		CloseHandle(hWriteFile);
	}

	//7. 显示出来
	this->InsertItem(m_iRow, lpName);
	this->SetItemText(m_iRow, PATH_COL, lpPath);
	this->SetItemText(m_iRow, STATUS_COL, (PathFileExists(lpPath)? TEXT("可用"):TEXT("不可用")));
	this->SetItemText(m_iRow, REMARK_COL, lpRemark);
	m_iRow++;
		
	return;
}


/*****************************************
功能：左键双击显示文件夹界面
*****************************************/
void CFastDirOpenDlg::OnNMDBClickListFastdir(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	TCHAR szDirPath[DEFAULT_SIZE] = { 0 };

	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));

	m_FastDirList.ScreenToClient(&point);

	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;

	int nItem = m_FastDirList.SubItemHitTest(&lvinfo);
	if (nItem >= 0 && nItem < m_FastDirList.m_iRow)
	{
		m_FastDirList.GetItemText(nItem, PATH_COL, szDirPath, DEFAULT_SIZE);
		m_FastDirList.m_iCurrentItem = nItem;

		if(PathFileExists(szDirPath))
			ShowDirAlways(szDirPath);
		else{
			m_FastDirList.SetItemText(nItem, STATUS_COL, TEXT("不可用"));
		}
	}

	*pResult = 0;
}


/*****************************************
功能：右键单击显示右键菜单
*****************************************/
void CFastDirOpenDlg::OnNMRClickListFastdir(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	CPoint pt;//右击菜单的右上角的位置是基于屏幕坐标系  

	CMenu m;
	
	do{
		//1. 检查是不是空行
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));

		m_FastDirList.ScreenToClient(&point);

		LVHITTESTINFO lvinfo;
		lvinfo.pt = point;
		lvinfo.flags = LVHT_ABOVE;

		int nItem = m_FastDirList.SubItemHitTest(&lvinfo);//获得当前点击的行号

		//2. 显示右键菜单
		GetCursorPos(&pt);//获得鼠标位置,屏幕位置
		//ClientToScreen(&pt);//如果坐标是窗口坐标则需要转换为屏幕坐标

		if(!m.LoadMenu(IDR_MENU_DELETE)){//加载菜单资源，IDR_MENU_DELETE的第一个菜单需要有下一个菜单才能TrackPopupMenu
			ThrowMes(TEXT("LoadMenu"));
			break;
		}

		if(!m.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this)){//显示菜单，这里的坐标是屏幕坐标
			ThrowMes(TEXT("TrackPopupMenu"));
			break;
		}

		m_FastDirList.m_iCurrentItem = nItem;
	}while(FALSE);

	*pResult = 0;
}


/*****************************************
功能：检查是否已经添加该路径this->
*****************************************/
BOOL CMyListCtrl::CheckRepeatDir(LPTSTR lpNewPath) {

	BOOL bRet = FALSE;
	TCHAR szTemp[DEFAULT_SIZE] = {0};

	for(int i = 0; i < m_iRow; i++){
		this->GetItemText(i, PATH_COL, szTemp, DEFAULT_SIZE);
		if(_tcsicmp(szTemp, lpNewPath) == 0){
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}


/*****************************************
功能：记录所有item
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

		for(int i = 0; i < m_FastDirList.m_iRow; i++){
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
功能：删除一项
*****************************************/
void CFastDirOpenDlg::DeleteOneItem(){
	// TODO: 在此添加命令处理程序代码	
	if (m_FastDirList.m_iCurrentItem < 0 || m_FastDirList.m_iCurrentItem >= m_FastDirList.m_iRow) {
		::MessageBox(NULL, TEXT("不能删除根本不存在的东西"), TEXT("DEL ERR"), MB_ICONWARNING | MB_OK);
	}
	else {
		TCHAR szName[DEFAULT_SIZE] = {0};
		TCHAR szPath[DEFAULT_SIZE] = { 0 };
		TCHAR szRemark[DEFAULT_SIZE] = { 0 };

		m_FastDirList.GetItemText(m_FastDirList.m_iCurrentItem, NAME_COL, szName, DEFAULT_SIZE);
		m_FastDirList.GetItemText(m_FastDirList.m_iCurrentItem, PATH_COL, szPath, DEFAULT_SIZE);
		m_FastDirList.GetItemText(m_FastDirList.m_iCurrentItem, REMARK_COL, szRemark, DEFAULT_SIZE);
		DeleteAnItem(INI_PATH, szName, szPath, szRemark);
		
		m_FastDirList.DeleteItem(m_FastDirList.m_iCurrentItem);
		m_FastDirList.m_iRow--;
		m_FastDirList.m_iCurrentItem = -1;
	}
}



/*****************************************
功能：获得当前鼠标指向的行，赋值给m_iCurrentItem
*****************************************/
void CFastDirOpenDlg::GetCurrentItemNum(){
	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));

	m_FastDirList.ScreenToClient(&point);

	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;

	m_FastDirList.m_iCurrentItem = m_FastDirList.SubItemHitTest(&lvinfo);

	return ;
}
