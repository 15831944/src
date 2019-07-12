#include "stdafx.h"
#include "CDiskFileTree.h"


CDiskFileTree::CDiskFileTree()
{
}

CDiskFileTree::~CDiskFileTree()
{
}


void CDiskFileTree::InitTreeRoot() {
	BOOL bRet = FALSE;
	TCHAR szDiskString[DSIZE] = { 0 };
	TCHAR szTmp[DSIZE] = { 0 };
	RtlZeroMemory(szDiskString, DSIZE * sizeof(TCHAR));

	do {
		int iLen = GetLogicalDriveStrings(DSIZE, szDiskString);
		if (0 == iLen) {
			break;
		}

		int iCount = iLen / 4;
		for (int i = 0; i < iCount; i++) {

			RtlZeroMemory(szTmp, DSIZE * sizeof(TCHAR));
			RtlCopyMemory(szTmp, szDiskString + i * 4, 2 * sizeof(TCHAR));

			TVINSERTSTRUCT tvInsert;
			tvInsert.hParent = NULL;
			tvInsert.hInsertAfter = NULL;
			tvInsert.item.mask = TVIF_TEXT | TVIF_CHILDREN;
			tvInsert.item.pszText = szTmp;
			tvInsert.item.cChildren = 1;// I_CHILDRENCALLBACK;
			//m_hRoot[szTmp[0]] =  this->
			InsertItem(&tvInsert);
		}
	} while (FALSE);
}

void CDiskFileTree::InitOpenDir() {
	HRESULT hr;
	HKEY hKey = NULL;
	do {

		hr = RegOpenKeyEx(HKEY_CURRENT_USER, REG_SUBKEY_NAME, 0, KEY_ALL_ACCESS, &hKey);
		if (hr != ERROR_SUCCESS) {
			DWORD dwDisposition;
			RegCreateKeyEx(HKEY_CURRENT_USER, REG_SUBKEY_NAME, 0, NULL, 0, KEY_READ | KEY_WRITE, NULL, &hKey, &dwDisposition);
			break;
		}

		TCHAR szDirPath[DSIZE] = { 0 };
		DWORD dwIndex = 0;  // 注册表项的键值索引 
		DWORD dwValueNameLen = DSIZE;   // 保存返回读取的字符长度 
		BYTE pbData[MAX_PATH] = { 0 }; //保存键的值 
		DWORD dwDataSize = MAX_PATH;  //保存值得长度 
		DWORD dwType = REG_SZ;	      //键值的类型 
		while (ERROR_SUCCESS == RegEnumValue(hKey, dwIndex, szDirPath, &dwValueNameLen, NULL, NULL, NULL, NULL)){
			dwValueNameLen = DSIZE;
			InitWholeTree(szDirPath);
			dwIndex++;
		}
	} while (FALSE);

	if (NULL != hKey)
		RegCloseKey(hKey);
}

void CDiskFileTree::AddOpenDirToReg(LPTSTR szDirPath) {
	HRESULT hr;
	HKEY hKey = NULL;
	do {
		hr = RegOpenKeyEx(HKEY_CURRENT_USER, REG_SUBKEY_NAME, 0, KEY_ALL_ACCESS, &hKey);
		if (hr != ERROR_SUCCESS) {
			DWORD dwDisposition;
			hr = RegCreateKeyEx(HKEY_CURRENT_USER, REG_SUBKEY_NAME, 0, NULL, 0, KEY_READ | KEY_WRITE, NULL, &hKey, &dwDisposition);
		}

		if (hr != ERROR_SUCCESS) {
			break;
		}

		RegSetValueEx(hKey, szDirPath, 0, REG_SZ, NULL, 0);
	} while (FALSE);
}

void CDiskFileTree::DelOpenDirInReg(LPTSTR szDirPath) {
	HRESULT hr;
	HKEY hKey = NULL;
	do {
		hr = RegOpenKeyEx(HKEY_CURRENT_USER, REG_SUBKEY_NAME, 0, KEY_ALL_ACCESS, &hKey);
		if (hr != ERROR_SUCCESS) { 
			break;
		}

		RegDeleteValue(hKey, szDirPath);

	} while (FALSE);

}

HTREEITEM CDiskFileTree::InsertSingleSubItem(HTREEITEM hParent, HTREEITEM hBro, LPTSTR szText, int ifAddImage) {
	HTREEITEM hRetHandle = NULL;
	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = hParent;
	tvInsert.hInsertAfter = hBro;
	tvInsert.item.mask = TVIF_TEXT | TVIF_CHILDREN;
	tvInsert.item.pszText = szText;
	if(ifAddImage)
		tvInsert.item.cChildren = 1;//I_CHILDRENCALLBACK;
	else
		tvInsert.item.cChildren = 0;// I_CHILDRENCALLBACK;
	return this->InsertItem(&tvInsert);
}

HTREEITEM CDiskFileTree::InsertSingleFileList(HTREEITEM hParent, LPTSTR szDir, LPTSTR szFullPath) {
	
	HTREEITEM hRetHandle = NULL;
	HTREEITEM hBro = TVI_LAST;

	BOOL bNext = TRUE;
	TCHAR szPat[DSIZE] = { 0 };
	lstrcpy(szPat, szDir);
	lstrcat(szPat, TEXT("\\*.*"));

	WIN32_FIND_DATA wfd;
	HANDLE hFile = FindFirstFile(szPat, &wfd);
	while (hFile != NULL && bNext == TRUE) {
		TCHAR lpTmp[DSIZE] = {0};
		RtlZeroMemory(lpTmp, DSIZE * sizeof(TCHAR));

		if (lstrcmp(TEXT("."), wfd.cFileName) != 0 && lstrcmp(TEXT(".."), wfd.cFileName) != 0 
			&& lstrcmp(TEXT("$RECYCLE.BIN"), wfd.cFileName) != 0
			&& lstrcmp(TEXT("System Volume Information"), wfd.cFileName) != 0) {
			hBro = InsertSingleSubItem(hParent, hBro, wfd.cFileName, wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
			::wsprintf(lpTmp, TEXT("%s\\%s"), szDir, wfd.cFileName);
			if (_tcsstr(szFullPath, lpTmp))
				hRetHandle = hBro;
		}

		bNext = FindNextFile(hFile, &wfd);
	}

	return hRetHandle;
}

HTREEITEM CDiskFileTree::FindSubItem(HTREEITEM hParent, LPTSTR szText) {

	HTREEITEM hChlidItem = GetChildItem(hParent);//得到指定树节点的子节点
	while (hChlidItem != NULL)
	{
		CString temp = GetItemText(hChlidItem);//获取节点上的文本
		if (lstrcmp(szText, temp.GetString()) == 0) {
			return hChlidItem;
		}
		hChlidItem = GetNextSiblingItem(hChlidItem);//得到子节点的兄弟节点
	}

	return NULL;
}

void CDiskFileTree::InitWholeTree(LPTSTR szFullPath) {
	TCHAR szTmp[DSIZE] = { 0 };
	TCHAR szText[DSIZE] = { 0 };
	HTREEITEM hParent = TVI_ROOT;// m_hRoot[szFullPath[0]];
	HTREEITEM hRetTreeItem = NULL;
	
	int iTotalLen = lstrlen(szFullPath);

	int j = 0;
	for(int i = 0; i < iTotalLen; i++){
		if (szFullPath[i] == TEXT('\\')) {
			//if (lstrcmp(szText, TEXT("F:")) == 0)
			//	printf("");

			hRetTreeItem = NULL;
			hRetTreeItem = FindSubItem(hParent, szText);

			j = 0;
			RtlZeroMemory(szText, DSIZE * sizeof(TCHAR));

			if (NULL == hRetTreeItem) {
				HTREEITEM hRetTreeItem = InsertSingleFileList(hParent, szTmp, szFullPath);
				Expand(hParent, TVE_EXPAND);
			
			}

			hParent = hRetTreeItem;
			if (NULL == GetChildItem(hParent)) {
				HTREEITEM hRetTreeItem = InsertSingleFileList(hParent, szTmp, szFullPath);
				Expand(hParent, TVE_EXPAND);
			}
		}
		else
			szText[j++] = szFullPath[i];

		szTmp[i] = szFullPath[i];
	}

	hRetTreeItem = FindSubItem(hParent, szText);
	if (NULL == GetChildItem(hRetTreeItem)) {
		hParent = hRetTreeItem;
		InsertSingleFileList(hParent, szFullPath, szFullPath);
		Expand(hParent, TVE_EXPAND);
	}
}

void CDiskFileTree::GetFullPath(HTREEITEM hItem, LPTSTR szFullPath) {
	HTREEITEM hParent = hItem;
	while (hParent) {
		TCHAR szTmp[DSIZE] = { 0 };
		wsprintf(szTmp, TEXT("%s"), szFullPath);

		CString csTmp = this->GetItemText(hParent);
		wsprintf(szFullPath, TEXT("%s\\%s"), csTmp.GetString(), szTmp);

		hParent = this->GetParentItem(hParent);
	}

	szFullPath[lstrlen(szFullPath) - 1] = 0;
}

void CDiskFileTree::UpDateWholeTree() {
	DeleteAllItems();
	InitTreeRoot();
	InitOpenDir();
}