
// FastDirOpenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FastDirOpen.h"
#include "FastDirOpenDlg.h"
#include "afxdialogex.h"

#include "AddDirPath.h"
#include "DialogChangeRemark.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFastDirOpenDlg �Ի���



CFastDirOpenDlg::CFastDirOpenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FASTDIROPEN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFastDirOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FASTDIR, m_FastDirList);
	DDX_Control(pDX, IDC_EDIT_REMARK, m_CEditRemark);
}

BEGIN_MESSAGE_MAP(CFastDirOpenDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FASTDIR, &CFastDirOpenDlg::OnNMDBClickListFastdir)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FASTDIR, &CFastDirOpenDlg::OnNMRClickListFastdir)

	ON_COMMAND(ID_RMENU_DELETE, &CFastDirOpenDlg::OnRCItemDelete)
	ON_COMMAND(ID_RMENU_ADD, &CFastDirOpenDlg::OnRmenuAdd)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CFastDirOpenDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_CLEARINVALID, &CFastDirOpenDlg::OnBnClickedButtonClearinvalid)
	ON_BN_CLICKED(IDC_BUTTON_CLEARALL, &CFastDirOpenDlg::OnBnClickedButtonClearall)
	ON_WM_SIZE()
	ON_COMMAND(ID_RCLICK_REMARK, &CFastDirOpenDlg::OnRclickRemark)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_FASTDIR, &CFastDirOpenDlg::OnLvnColumnclickListFastdir)

	ON_MESSAGE(WM_MY_SHOWTASK, OnMyShowTask)//[��С��]3.��Ϣע��

	ON_WM_TIMER()

	ON_NOTIFY(NM_CLICK, IDC_LIST_FASTDIR, &CFastDirOpenDlg::OnNMClickListFastdir)
	ON_EN_KILLFOCUS(IDC_EDIT_REMARK, &CFastDirOpenDlg::OnEnKillfocusEditRemark)
END_MESSAGE_MAP()


//[��С��]4.��Ϣ����ʵ��
LRESULT CFastDirOpenDlg::OnMyShowTask(WPARAM wParam, LPARAM lParam){
	
	BOOL bIsWndMin = FALSE;

	if (wParam != IDR_MAINFRAME)
		return 1;
	switch (lParam){

	case WM_RBUTTONUP:{ //���˴����Ų��������ﶨ�����      // �Ҽ�����ʱ�����˵� 
		LPPOINT lpoint = new tagPOINT;
		::GetCursorPos(lpoint);                    // �õ����λ��
		CMenu menu;
		menu.CreatePopupMenu();                    // ����һ������ʽ�˵�
		menu.AppendMenu(MF_STRING, WM_DESTROY, TEXT("�ر�"));//�Ҽ��˵�����������
		menu.AppendMenu(MF_STRING, WM_CLOSE, TEXT("ȡ��"));//�Ҽ��˵�����������
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
		HMENU hmenu = menu.Detach();
		menu.DestroyMenu();
		delete lpoint;
		break;
	}

	case WM_LBUTTONUP: // ������Ĵ���
		bIsWndMin = this->IsWindowVisible();
		if(FALSE == bIsWndMin){
			this->ShowWindow(SW_SHOWNORMAL);         // ��ʾ������
		}
		else{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			//this->ShowWindow(SW_HIDE);         // ����������
		}
		break;
	}
	return 0;
}


//[��С��]7. ���ش�����Ϣ�����������ӣ���������С����������ͼ�����أ�����رմ�����ɾ��ϵͳ��������ͼ��
LRESULT CFastDirOpenDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message) //�ж���Ϣ����
	{
	case WM_SYSCOMMAND:
		//�����ϵͳ��Ϣ 
		if (wParam == SC_MINIMIZE)//��С��������Ϣ
		{
			//���յ���С����Ϣʱ���������� 
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
			return 0;
		}
		if (wParam == SC_CLOSE)
		{
			::Shell_NotifyIcon(NIM_DELETE, &m_nid); //�ر�ʱɾ��ϵͳ����ͼ��
		}
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}


// CFastDirOpenDlg ��Ϣ�������

BOOL CFastDirOpenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//1. ��ʼ��CListCtrl
	InitDirList();

	//2. ���ͼ�굽ϵͳ����
	//[��С��]6.����Shell_NotifyIcon���ͼ�굽ϵͳ����

	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->m_hWnd;
	m_nid.uID = IDR_MAINFRAME;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;//������С����ϵͳ����ʱҪ��icon,������Ӧ��Ϣ��������ʾtips
	m_nid.uCallbackMessage = WM_MY_SHOWTASK;             // �Զ������Ϣ����

	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	_tcscpy(m_nid.szTip, TEXT("FAST DIR OPEN"));                // ��Ϣ��ʾ��Ϊ"����������"��VS2008 UNICODE������wcscpy_s()����

	Shell_NotifyIcon(NIM_ADD, &m_nid);                // �����������ͼ��

	//3. ��ȡ�߿��С
	CRect ClientRect;
	GetClientRect(&ClientRect);     //ȡ�ͻ�����С    

	CRect ItemRect;
	int woc = IDC_LIST_FASTDIR;

	GetDlgItem(woc)->GetWindowRect(ItemRect);
	//ClientToScreen(ItemRect);

	m_left = 20;//ItemRect.left - ClientRect.left;
	m_top = 52;//ItemRect.top - ClientRect.top;

	m_right = -20;//ItemRect.right - ClientRect.right;
	m_bottom = -20;//ItemRect.bottom - ClientRect.bottom;

	//4. ����remark�༭��
	m_CEditRemark.ShowWindow(HIDE_WINDOW);
	m_CEditRemark.EnableWindow(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}



// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFastDirOpenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFastDirOpenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*****************************************
���ܣ���Ӧ�Ҽ��˵�delete
*****************************************/
void CFastDirOpenDlg::OnRCItemDelete()
{
	DeleteOneItem();
}

/*****************************************
���ܣ���Ӧ�Ҽ��˵�add
*****************************************/
void CFastDirOpenDlg::OnRmenuAdd()
{
	// TODO: �ڴ���������������
	CAddDirPath dlgAddDir;

	TCHAR szPath[DEFAULT_SIZE] = { 0 };
	TCHAR szName[DEFAULT_SIZE] = {0};
	TCHAR szRemark[DEFAULT_SIZE] = TEXT("NULL");

	while (IDOK == dlgAddDir.DoModal()){
		if(!_tcslen(dlgAddDir.m_szPath))
			break;

		_tcscpy(szPath, dlgAddDir.m_szPath);

		TCHAR *pos = _tcsrchr(szPath, TEXT('\\'));
		_tcscpy(szName, pos + 1);

		if(_tcslen(dlgAddDir.m_szRemark))
			_tcscpy(szRemark, dlgAddDir.m_szRemark);
		
		AddOneDir(szName, szPath, szRemark);

		break;
	}
}


/*****************************************
���ܣ�ˢ���ļ���״̬
*****************************************/
void CFastDirOpenDlg::OnBnClickedButtonUpdate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szTemp[DEFAULT_SIZE] = {0};

	for(int i = 0; i < m_iRow; i++){
		m_FastDirList.GetItemText(i, PATH_COL, szTemp, DEFAULT_SIZE);

		if(PathFileExists(szTemp)){
			m_FastDirList.SetItemText(i, STATUS_COL, TEXT("����"));
		}
		else{
			m_FastDirList.SetItemText(i, STATUS_COL, TEXT("������"));
		}
	}
}



/*****************************************
���ܣ�������Ч�ļ���
*****************************************/
void CFastDirOpenDlg::OnBnClickedButtonClearinvalid()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ClearInvaildDir();

	WriteAllItems();//�ǵð������������д��ini�ļ�
}


/*****************************************
���ܣ�����б�
*****************************************/
void CFastDirOpenDlg::OnBnClickedButtonClearall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ClearAllDir();

	WriteAllItems();//�ǵð�����������д��ini�ļ�
}


void CFastDirOpenDlg::ReSize(void)
{
	CRect NewRect;
	GetClientRect(&NewRect);     //ȡ�ͻ�����С    

	CRect Rect;
	int woc = IDC_LIST_FASTDIR;
	CPoint TLPoint; //���Ͻ�  
	CPoint BRPoint; //���½�  

	if(GetDlgItem(woc)){

		TLPoint.x = NewRect.left + m_left;
		TLPoint.y = NewRect.top + m_top;

		BRPoint.x = NewRect.right + m_right;
		BRPoint.y = NewRect.bottom + m_bottom;

		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
	}
}


void CFastDirOpenDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType != SIZE_MINIMIZED)  //�жϴ����ǲ�����С���ˣ���Ϊ������С��֮�� �����ڵĳ��Ϳ����0����ǰһ�α仯��ʱ�ͻ���ֳ���0�Ĵ������
	{
		ReSize();
	}
}



/*****************************************
���ܣ��޸ı�ע
*****************************************/
void CFastDirOpenDlg::OnRclickRemark(){

	CDialogChangeRemark dlgChangeRemark;

	//1. �����Ҽ���Ϣʱ�Ѿ��������item��ֵ�ˣ��������Get���ǡ���ע������item����
	//���Ҽ���Ϣ������к������жϣ���������Ҫ�����ж�
	if(m_iCurrentItem < 0 || m_iCurrentItem >= m_iRow){
		::MessageBox(NULL, TEXT("���ܵ�������ڵ�item"), TEXT("CLICK ERR"), MB_ICONERROR | MB_OK);
		return ;
	}

	//2. ����޸�����
	while (IDOK == dlgChangeRemark.DoModal()) {

		if (!_tcslen(dlgChangeRemark.m_szRemark)){
			m_FastDirList.SetItemText(m_iCurrentItem, REMARK_COL, TEXT("NULL"));
		}
		else
			m_FastDirList.SetItemText(m_iCurrentItem, REMARK_COL, dlgChangeRemark.m_szRemark);

		WriteAllItems();//����͵���ˣ��޸���remark��ֱ����д��һ��ini�ļ�

		break;
	}
}


//������Ϣ����������esc��enter������Ϊ��escĬ�ϵ���OnCancel��enterĬ�ϵ���OnOK(��ȻҲ������������������������esc��enter)
//�����������������������������ʱ���˳�����
BOOL CFastDirOpenDlg::PreTranslateMessage(MSG* pMsg)
{
	//if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_ESCAPE)     return   TRUE; //Ϊ���˳����㣬�Ҹ������ˣ�������ESC��
	if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_RETURN){

		if(m_CEditRemark.IsWindowEnabled()){//������޸ı�ע�س������޸ı�ע
			OnGetEidtRemark(m_iCurRemark);
			m_iCurRemark = -1;
			m_CEditRemark.ShowWindow(FALSE);
			m_CEditRemark.EnableWindow(FALSE);
		}

		return   TRUE;
	}
	else
		return   CDialog::PreTranslateMessage(pMsg);

}



/*****************************************
���ܣ�����ص����������ڱȽ�����������Ĵ�С;�ص�����������
*****************************************/
int CALLBACK Compare2Items(LPARAM lParam1, LPARAM lParam2, LPARAM lParamThisDialog){

	TCHAR szColData1[DEFAULT_SIZE] = {0};
	TCHAR szColData2[DEFAULT_SIZE] = { 0 };

	int iCol = ((CFastDirOpenDlg *)lParamThisDialog)->m_iCurrentCol;

	((CFastDirOpenDlg *)lParamThisDialog)->m_FastDirList.GetItemText((int)lParam1, iCol, szColData1, DEFAULT_SIZE);
	((CFastDirOpenDlg *)lParamThisDialog)->m_FastDirList.GetItemText((int)lParam2, iCol, szColData2, DEFAULT_SIZE);

	return _tcscmp(szColData1, szColData2) > 0 ? 1 : 0;
}


/*****************************************
���ܣ����ĳһ�У������е�ֵ��������
*****************************************/
void CFastDirOpenDlg::OnLvnColumnclickListFastdir(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//1. ��õ�����к�
	m_iCurrentCol = pNMLV->iSubItem;//�������

	//2. ����item�󶨵�data(��ʵ��֪��Ϊʲô������)
	for(int i = 0; i < m_iRow; i++){
		m_FastDirList.SetItemData(i, i);
	}

	//3. ���ݻص������ıȽϽ����������
	m_FastDirList.SortItems((PFNLVCOMPARE)Compare2Items, (DWORD_PTR)this);

	//4. ����ˢ��ini�ļ�
	WriteAllItems();

	*pResult = 0;
}


//ʱ���ѹ���remark���ٵȴ��޸�
void CFastDirOpenDlg::OnTimer(UINT_PTR uIDEvent){
	KillTimer(REMARK_TIMER);
	if(FALSE == m_CEditRemark.IsWindowEnabled())
		m_iCurRemark = -1;
}


/****************************************************
���ܣ���Ӧ�����¼�-������ָ��Ӧremark�޸ĵĵ����¼�
****************************************************/
void CFastDirOpenDlg::OnNMClickListFastdir(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	DWORD dwPos = GetMessagePos();
	CPoint cPoint = {LOWORD(dwPos), HIWORD(dwPos)};

	m_FastDirList.ScreenToClient(&cPoint);

	LVHITTESTINFO lvinfo;
	lvinfo.pt = cPoint;
	lvinfo.flags = LVHT_ABOVE;

	int nItem = m_FastDirList.SubItemHitTest(&lvinfo);

	do{
		if(nItem == -1){//û�л���к�
			break;
		}

		if (lvinfo.iSubItem != REMARK_COL){//����Ĳ��Ǳ�ע��
			break;
		}

		if (m_iCurRemark != nItem) {//����������У���������н���ȴ�״̬

			m_iCurRemark = nItem;
			SetTimer(REMARK_TIMER, RM_WAIT_TIME, NULL);
		}
		else {
			//0. �Ѿ��ٴε���������޸�״̬�Ͳ����ټ�ʱ��
			KillTimer(REMARK_TIMER);

			//1.�趨�༭���λ��
			CRect cEditRect;
			CRect cListRect;
			m_FastDirList.GetSubItemRect(nItem, REMARK_COL, LVIR_LABEL, cEditRect);//�˴���õ��������m_FastDirList��λ�ã���movewindowʹ�õ������������client��λ�ã����Ի�Ҫ���б任
			m_FastDirList.GetWindowRect(&cListRect);

			ScreenToClient(&cListRect);
			cEditRect.left += cListRect.left;
			cEditRect.top += cListRect.top;
			cEditRect.right += cListRect.left;
			cEditRect.bottom += cListRect.top + 3;

			//2. �༭�������ʾ������
			TCHAR szOrgText[DEFAULT_SIZE] = {0};
			m_FastDirList.GetItemText(nItem, REMARK_COL, szOrgText, DEFAULT_SIZE);
			m_CEditRemark.SetWindowText(szOrgText);

			//3.�ƶ��༭��
			m_CEditRemark.MoveWindow(cEditRect);
			m_CEditRemark.EnableWindow(TRUE);
			m_CEditRemark.ShowWindow(SW_SHOW);
			m_CEditRemark.SetFocus();
			m_CEditRemark.SetSel(-1);

		}

	}while(FALSE);

	*pResult = 0;
}


/*******************************************
���ܣ�����ע�༭��༭��ɺ��޸Ķ�Ӧ��remark������дһ��ini�ļ�
*******************************************/
void CFastDirOpenDlg::OnGetEidtRemark(int nItem){

	TCHAR szNewText[DEFAULT_SIZE] = { 0 };
	m_CEditRemark.GetWindowText(szNewText, DEFAULT_SIZE);
	if(_tcslen(szNewText))
		m_FastDirList.SetItemText(nItem, REMARK_COL, szNewText);
	else
		m_FastDirList.SetItemText(nItem, REMARK_COL, TEXT("NULL"));

	WriteAllItems();
}


/*********************************************
���ܣ������ע�༭�����뽹����ʧ������Ϊ�༭��ɣ��޸ı�ע��ini����icurremark = -1
*********************************************/
void CFastDirOpenDlg::OnEnKillfocusEditRemark()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnGetEidtRemark(m_iCurRemark);
	m_iCurRemark = -1;
	m_CEditRemark.ShowWindow(FALSE);
	m_CEditRemark.EnableWindow(FALSE);
}
