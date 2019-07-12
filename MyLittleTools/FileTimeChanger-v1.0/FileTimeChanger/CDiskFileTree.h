#pragma once
#include <afxcmn.h>
#include "stdafx.h"


class CDiskFileTree :
	public CTreeCtrl
{
public:
	CDiskFileTree();
	~CDiskFileTree();

public:
	//HTREEITEM m_hRoot[26];

	void InitTreeRoot(); 
	void InitOpenDir();
	
	void AddOpenDirToReg(LPTSTR szDirPath);
	void DelOpenDirInReg(LPTSTR szDirPath);

	HTREEITEM InsertSingleSubItem(HTREEITEM hParent, HTREEITEM hBro, LPTSTR szText, int ifAddImage);
	HTREEITEM InsertSingleFileList(HTREEITEM hParent, LPTSTR szDir, LPTSTR szFullPath);
	void InitWholeTree(LPTSTR szFullPath);
	HTREEITEM FindSubItem(HTREEITEM hParent, LPTSTR szText);

	void GetFullPath(HTREEITEM hItem, LPTSTR szFullPath);
	void UpDateWholeTree();
};

#define REG_SUBKEY_NAME TEXT("FileTimeChanger\\OpenDir")//FileTimeChanger\\OpenDir"