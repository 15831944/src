
// EasyTreeHoleDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "EasyTreeHole.h"
#include "EasyTreeHoleDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEasyTreeHoleDlg 对话框



CEasyTreeHoleDlg::CEasyTreeHoleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EASYTREEHOLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEasyTreeHoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILEPATH, mFilePath);
	DDX_Control(pDX, IDC_EDIT_AESKEY, mAESKey);
	DDX_Control(pDX, IDC_EDIT_DATA, mData);
}

BEGIN_MESSAGE_MAP(CEasyTreeHoleDlg, CDialogEx)
	ON_WM_DROPFILES()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BUTTON_SAVE, &CEasyTreeHoleDlg::OnBnClickedSave)
	ON_BN_CLICKED(ID_BUTTON_OPEN, &CEasyTreeHoleDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_CHECK2, &CEasyTreeHoleDlg::OnBnClickedCheckPwShow)
	ON_BN_CLICKED(IDC_BUTTON_FILELIST, &CEasyTreeHoleDlg::OnBnClickedButtonFilelist)
END_MESSAGE_MAP()


// CEasyTreeHoleDlg 消息处理程序

BOOL CEasyTreeHoleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	mFilePath.mFilePathEdit = &mFilePath;
	mAESKey.mFilePathEdit = &mFilePath;
	mData.mFilePathEdit = &mFilePath;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEasyTreeHoleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEasyTreeHoleDlg::OnPaint()
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
HCURSOR CEasyTreeHoleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEasyTreeHoleDlg::OnDropFiles(HDROP hDropInfo)
{
	if (hDropInfo)//拖拽消息指针
	{
		TCHAR szFilePath[PATH_LEN] = { 0 };
		int nDrag = DragQueryFile(hDropInfo, -1, NULL, 0);//获取一共拖拽的文件的个数

		for (int i = 0; i < nDrag; i++) {
			RtlZeroMemory(szFilePath, PATH_LEN);
			DragQueryFile(hDropInfo, i, szFilePath, 1024);//获得第一个文件的路径m_szFilePath，返回路径长度

			mFilePath.SetWindowTextW(szFilePath);
		}
	}

	DragFinish(hDropInfo);
}

bool CheckFileType(wchar_t *szFileHead)
{
	int iLen = wcslen(FILE_HEAD);
	wchar_t *szHead = new wchar_t[iLen + 1];
	RtlZeroMemory(szHead, (iLen + 1)*2);
	wcsncpy_s(szHead, iLen + 1, szFileHead, iLen);

	if (wcscmp(szHead, FILE_HEAD) == 0)
		return TRUE;
	return FALSE;
}

int CEasyTreeHoleDlg::SaveFile(wchar_t *wszFilePath, unsigned char *aesKey, byte *byteInData, int iInSize)
{
	int iRet = 0;
	HANDLE hFile = CreateFileW(wszFilePath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	byte *byteOut = NULL;
	int iOutSize = AESCrypto(aesKey, (byte *)byteInData, iInSize, &byteOut);
	if (iOutSize > 0) {
		DWORD dwWrite = 0;
		if (TRUE == WriteFile(hFile, byteOut, iOutSize, &dwWrite, NULL))
		{
			iRet = dwWrite;
		}
	}
	if (byteOut)
		delete[] byteOut;

	CloseHandle(hFile);
	return iRet;
}

int CEasyTreeHoleDlg::ReadFromFile(wchar_t *wszFilePath, unsigned char *aesKey, byte **byteOut)
{
	int iRet = 0;
	HANDLE hFile = CreateFileW(wszFilePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	LARGE_INTEGER li = { 0 };
	GetFileSizeEx(hFile, &li);
	byte *byteCryptData = new byte[li.QuadPart];
	if (byteCryptData == NULL) {
		CloseHandle(hFile);
		return 0;
	}

	DWORD dwRead = 0;
	if (TRUE == ReadFile(hFile, byteCryptData, li.QuadPart, &dwRead, NULL)) {
		*byteOut = NULL;
		iRet = AESDeCrypto(aesKey, byteCryptData, li.QuadPart, byteOut);
	}
	if (byteCryptData == NULL)
		delete[] byteCryptData;

	CloseHandle(hFile);
	return iRet;
}

void  CEasyTreeHoleDlg::PrintUnlockData()
{
	wchar_t wszFilePath[PATH_LEN] = { 0 };
	mFilePath.GetWindowTextW(wszFilePath, PATH_LEN);
	if (!PathFileExistsW(wszFilePath)) 
	{
		return;
	}

	wchar_t waesKey[17] = { 0 };
	unsigned char aesKey[17] = { 0 };
	mAESKey.GetWindowTextW(waesKey, 17);
	WideCharToMultiByte(CP_ACP, 0, waesKey, wcslen(waesKey), (char *)aesKey, 17, NULL, NULL);

	byte *wszData = NULL;
	int iRet = ReadFromFile(wszFilePath, aesKey, &wszData);

	if (TRUE == CheckFileType((wchar_t*)wszData))
		mData.SetWindowTextW((wchar_t*)wszData + wcslen(FILE_HEAD));
	else
		MessageBoxA(NULL, "THIS FILE IS NOT LOACKED BY ME.", "ERROR", MB_OK|MB_ICONERROR);
}

void CEasyTreeHoleDlg::OnBnClickedSave()
{
	if (mFilePath.GetWindowTextLengthW() > 0 && mAESKey.GetWindowTextLengthW() > 0)
	{
		wchar_t wszFilePath[PATH_LEN] = { 0 };
		mFilePath.GetWindowTextW(wszFilePath, PATH_LEN);

		int iDataSize = mData.GetWindowTextLengthW() + wcslen(FILE_HEAD) + 1;
		wchar_t *wszData = new wchar_t[iDataSize];
		if (wszData == NULL)
			return;

		wcscpy_s(wszData, iDataSize, FILE_HEAD);

		mData.GetWindowTextW(wszData + wcslen(FILE_HEAD), iDataSize - wcslen(FILE_HEAD) + 1);

		wchar_t waesKey[17] = { 0 };
		unsigned char aesKey[17] = { 0 };
		mAESKey.GetWindowTextW(waesKey, 17);
		WideCharToMultiByte(CP_ACP, 0, waesKey, wcslen(waesKey), (char *)aesKey, 17, NULL, NULL);
		SaveFile(wszFilePath, aesKey, (BYTE*)wszData, iDataSize * 2 + 2);
		if(wszData)
			delete[] wszData;
		MessageBoxW(L"SAVE SUCCEEDED", L"SAVE", MB_OK);
	}
	else
	{
		MessageBoxW(L"WRONG ARG", L"ERROR", MB_OK | MB_ICONERROR);
	}
}

void CEasyTreeHoleDlg::OnBnClickedButtonOpen()
{
	if (mFilePath.GetWindowTextLengthW() > 0 && mAESKey.GetWindowTextLengthW() > 0)
	{
		PrintUnlockData();
	}
	else
	{
		MessageBoxW(L"WRONG ARG", L"ERROR", MB_OK|MB_ICONERROR);
	}
}

BOOL CEasyTreeHoleDlg::PreTranslateMessage(MSG* pMsg)
{
	if ((GetKeyState(VK_CONTROL) & 0x8000))
	{
		switch (pMsg->wParam)
		{
		case 'S':
		case 's':
			OnBnClickedSave();
			break;
		default:
			break;
		}

		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CEasyTreeHoleDlg::OnBnClickedCheckPwShow()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_PW))
	{
		mAESKey.SetPasswordChar(NULL);
		mAESKey.SetFocus();
	}
	else
	{
		mAESKey.SetPasswordChar('*');
		mAESKey.SetFocus();
	}

}


void CEasyTreeHoleDlg::OnBnClickedButtonFilelist()
{
	// "*.my" for "MyType Files" and "*.*' for "All Files."
	TCHAR szFilters[] = _T("All Files (*.*)|*.*||");//神一样的格式eg：MyType Files (*.my)|*.my|All Files (*.*)|*.*|| =》描述|文件类型过滤符号|| 以“|”分割，以“||”做结尾

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg(TRUE, _T("ALL"), _T("*.*"),
		OFN_FILEMUSTEXIST, szFilters);

	// use DoModal() to Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if (fileDlg.DoModal() == IDOK)
	{
		CStringW pathName = fileDlg.GetPathName();

		mFilePath.SetWindowTextW(pathName);
	}
}