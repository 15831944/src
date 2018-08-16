
// FastDirOpenDlg.cpp : 实现文件
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


// CFastDirOpenDlg 对话框



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

	ON_MESSAGE(WM_MY_SHOWTASK, OnMyShowTask)//[最小化]3.消息注册

	ON_WM_TIMER()

	ON_NOTIFY(NM_CLICK, IDC_LIST_FASTDIR, &CFastDirOpenDlg::OnNMClickListFastdir)
	ON_EN_KILLFOCUS(IDC_EDIT_REMARK, &CFastDirOpenDlg::OnEnKillfocusEditRemark)
END_MESSAGE_MAP()


//[最小化]4.消息函数实现
LRESULT CFastDirOpenDlg::OnMyShowTask(WPARAM wParam, LPARAM lParam){
	
	BOOL bIsWndMin = FALSE;

	if (wParam != IDR_MAINFRAME)
		return 1;
	switch (lParam){

	case WM_RBUTTONUP:{ //加了大括号才能在这里定义变量      // 右键起来时弹出菜单 
		LPPOINT lpoint = new tagPOINT;
		::GetCursorPos(lpoint);                    // 得到鼠标位置
		CMenu menu;
		menu.CreatePopupMenu();                    // 声明一个弹出式菜单
		menu.AppendMenu(MF_STRING, WM_DESTROY, TEXT("关闭"));//右键菜单还可以这样
		menu.AppendMenu(MF_STRING, WM_CLOSE, TEXT("取消"));//右键菜单还可以这样
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
		HMENU hmenu = menu.Detach();
		menu.DestroyMenu();
		delete lpoint;
		break;
	}

	case WM_LBUTTONUP: // 击左键的处理
		bIsWndMin = this->IsWindowVisible();
		if(FALSE == bIsWndMin){
			this->ShowWindow(SW_SHOWNORMAL);         // 显示主窗口
		}
		else{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			//this->ShowWindow(SW_HIDE);         // 隐藏主窗口
		}
		break;
	}
	return 0;
}


//[最小化]7. 重载窗口消息处理函数，增加：当窗口最小化将任务栏图标隐藏，如果关闭窗口则删除系统任务栏中图标
LRESULT CFastDirOpenDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message) //判断消息类型
	{
	case WM_SYSCOMMAND:
		//如果是系统消息 
		if (wParam == SC_MINIMIZE)//最小化窗口消息
		{
			//接收到最小化消息时主窗口隐藏 
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
			return 0;
		}
		if (wParam == SC_CLOSE)
		{
			::Shell_NotifyIcon(NIM_DELETE, &m_nid); //关闭时删除系统托盘图标
		}
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}


// CFastDirOpenDlg 消息处理程序

BOOL CFastDirOpenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//1. 初始化CListCtrl
	InitDirList();

	//2. 添加图标到系统托盘
	//[最小化]6.调用Shell_NotifyIcon添加图标到系统托盘

	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->m_hWnd;
	m_nid.uID = IDR_MAINFRAME;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;//程序最小化到系统托盘时要有icon,可以相应消息，可以显示tips
	m_nid.uCallbackMessage = WM_MY_SHOWTASK;             // 自定义的消息名称

	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	_tcscpy(m_nid.szTip, TEXT("FAST DIR OPEN"));                // 信息提示条为"服务器程序"，VS2008 UNICODE编码用wcscpy_s()函数

	Shell_NotifyIcon(NIM_ADD, &m_nid);                // 在托盘区添加图标

	//3. 获取边框大小
	CRect ClientRect;
	GetClientRect(&ClientRect);     //取客户区大小    

	CRect ItemRect;
	int woc = IDC_LIST_FASTDIR;

	GetDlgItem(woc)->GetWindowRect(ItemRect);
	//ClientToScreen(ItemRect);

	m_left = 20;//ItemRect.left - ClientRect.left;
	m_top = 52;//ItemRect.top - ClientRect.top;

	m_right = -20;//ItemRect.right - ClientRect.right;
	m_bottom = -20;//ItemRect.bottom - ClientRect.bottom;

	//4. 构建remark编辑框
	m_CEditRemark.ShowWindow(HIDE_WINDOW);
	m_CEditRemark.EnableWindow(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFastDirOpenDlg::OnPaint()
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
HCURSOR CFastDirOpenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*****************************************
功能：响应右键菜单delete
*****************************************/
void CFastDirOpenDlg::OnRCItemDelete()
{
	DeleteOneItem();
}

/*****************************************
功能：响应右键菜单add
*****************************************/
void CFastDirOpenDlg::OnRmenuAdd()
{
	// TODO: 在此添加命令处理程序代码
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
功能：刷新文件夹状态
*****************************************/
void CFastDirOpenDlg::OnBnClickedButtonUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szTemp[DEFAULT_SIZE] = {0};

	for(int i = 0; i < m_iRow; i++){
		m_FastDirList.GetItemText(i, PATH_COL, szTemp, DEFAULT_SIZE);

		if(PathFileExists(szTemp)){
			m_FastDirList.SetItemText(i, STATUS_COL, TEXT("可用"));
		}
		else{
			m_FastDirList.SetItemText(i, STATUS_COL, TEXT("不可用"));
		}
	}
}



/*****************************************
功能：清理无效文件夹
*****************************************/
void CFastDirOpenDlg::OnBnClickedButtonClearinvalid()
{
	// TODO: 在此添加控件通知处理程序代码
	ClearInvaildDir();

	WriteAllItems();//记得把清理完的重新写入ini文件
}


/*****************************************
功能：清空列表
*****************************************/
void CFastDirOpenDlg::OnBnClickedButtonClearall()
{
	// TODO: 在此添加控件通知处理程序代码
	ClearAllDir();

	WriteAllItems();//记得把清空完的重新写入ini文件
}


void CFastDirOpenDlg::ReSize(void)
{
	CRect NewRect;
	GetClientRect(&NewRect);     //取客户区大小    

	CRect Rect;
	int woc = IDC_LIST_FASTDIR;
	CPoint TLPoint; //左上角  
	CPoint BRPoint; //右下角  

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
	if (nType != SIZE_MINIMIZED)  //判断窗口是不是最小化了，因为窗口最小化之后 ，窗口的长和宽会变成0，当前一次变化的时就会出现除以0的错误操作
	{
		ReSize();
	}
}



/*****************************************
功能：修改备注
*****************************************/
void CFastDirOpenDlg::OnRclickRemark(){

	CDialogChangeRemark dlgChangeRemark;

	//1. 处理右键消息时已经将点击的item赋值了；如果现在Get则是“备注”所在item的行
	//但右键信息不会进行合理性判断，所以这里要进行判断
	if(m_iCurrentItem < 0 || m_iCurrentItem >= m_iRow){
		::MessageBox(NULL, TEXT("不能点击不存在的item"), TEXT("CLICK ERR"), MB_ICONERROR | MB_OK);
		return ;
	}

	//2. 获得修改内容
	while (IDOK == dlgChangeRemark.DoModal()) {

		if (!_tcslen(dlgChangeRemark.m_szRemark)){
			m_FastDirList.SetItemText(m_iCurrentItem, REMARK_COL, TEXT("NULL"));
		}
		else
			m_FastDirList.SetItemText(m_iCurrentItem, REMARK_COL, dlgChangeRemark.m_szRemark);

		WriteAllItems();//这里偷懒了，修改完remark后直接重写了一遍ini文件

		break;
	}
}


//重载消息函数：屏蔽esc和enter键，因为按esc默认调用OnCancel，enter默认调用OnOK(当然也可以重载这两个函数来屏蔽esc和enter)
//如果不屏蔽这两个键，则按这两个键时会退出程序
BOOL CFastDirOpenDlg::PreTranslateMessage(MSG* pMsg)
{
	//if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_ESCAPE)     return   TRUE; //为了退出方便，我改主意了，不屏蔽ESC了
	if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_RETURN){

		if(m_CEditRemark.IsWindowEnabled()){//如果是修改备注回车，则修改备注
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
功能：排序回调函数，用于比较两个相邻项的大小;回调函数不能是
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
功能：点击某一列，按该列的值进行排序
*****************************************/
void CFastDirOpenDlg::OnLvnColumnclickListFastdir(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//1. 获得点击的列号
	m_iCurrentCol = pNMLV->iSubItem;//点击的列

	//2. 设置item绑定的data(其实不知道为什么这样做)
	for(int i = 0; i < m_iRow; i++){
		m_FastDirList.SetItemData(i, i);
	}

	//3. 根据回调函数的比较结果进行排序
	m_FastDirList.SortItems((PFNLVCOMPARE)Compare2Items, (DWORD_PTR)this);

	//4. 排完刷新ini文件
	WriteAllItems();

	*pResult = 0;
}


//时间已过，remark不再等待修改
void CFastDirOpenDlg::OnTimer(UINT_PTR uIDEvent){
	KillTimer(REMARK_TIMER);
	if(FALSE == m_CEditRemark.IsWindowEnabled())
		m_iCurRemark = -1;
}


/****************************************************
功能：响应单击事件-具体是指响应remark修改的单击事件
****************************************************/
void CFastDirOpenDlg::OnNMClickListFastdir(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	DWORD dwPos = GetMessagePos();
	CPoint cPoint = {LOWORD(dwPos), HIWORD(dwPos)};

	m_FastDirList.ScreenToClient(&cPoint);

	LVHITTESTINFO lvinfo;
	lvinfo.pt = cPoint;
	lvinfo.flags = LVHT_ABOVE;

	int nItem = m_FastDirList.SubItemHitTest(&lvinfo);

	do{
		if(nItem == -1){//没有获得行号
			break;
		}

		if (lvinfo.iSubItem != REMARK_COL){//点击的不是备注列
			break;
		}

		if (m_iCurRemark != nItem) {//点击了其他行，被点击的行进入等待状态

			m_iCurRemark = nItem;
			SetTimer(REMARK_TIMER, RM_WAIT_TIME, NULL);
		}
		else {
			//0. 已经再次点击进入了修改状态就不必再计时了
			KillTimer(REMARK_TIMER);

			//1.设定编辑框的位置
			CRect cEditRect;
			CRect cListRect;
			m_FastDirList.GetSubItemRect(nItem, REMARK_COL, LVIR_LABEL, cEditRect);//此处获得的是相对于m_FastDirList的位置，而movewindow使用的是相对于整个client的位置，所以还要进行变换
			m_FastDirList.GetWindowRect(&cListRect);

			ScreenToClient(&cListRect);
			cEditRect.left += cListRect.left;
			cEditRect.top += cListRect.top;
			cEditRect.right += cListRect.left;
			cEditRect.bottom += cListRect.top + 3;

			//2. 编辑框最初显示的文字
			TCHAR szOrgText[DEFAULT_SIZE] = {0};
			m_FastDirList.GetItemText(nItem, REMARK_COL, szOrgText, DEFAULT_SIZE);
			m_CEditRemark.SetWindowText(szOrgText);

			//3.移动编辑框
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
功能：当备注编辑框编辑完成后，修改对应的remark，并且写一遍ini文件
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
功能：如果备注编辑框输入焦点消失，则认为编辑完成，修改备注和ini，将icurremark = -1
*********************************************/
void CFastDirOpenDlg::OnEnKillfocusEditRemark()
{
	// TODO: 在此添加控件通知处理程序代码
	OnGetEidtRemark(m_iCurRemark);
	m_iCurRemark = -1;
	m_CEditRemark.ShowWindow(FALSE);
	m_CEditRemark.EnableWindow(FALSE);
}
